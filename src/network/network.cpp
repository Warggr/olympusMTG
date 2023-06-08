#include "network.h"
#include "logging.h"
#include <istream>
#include <sstream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>

/** The Olympus File Protocol:
 * A file consists of one or more chunks (like lines, except they're not separated by any special character);
 * the beginning and end of a chunk is understood purely through semantics.
 * Chunks are not split up through transmission blocks.
 * Chunks are written consecutively to each other.
 * At the end of a transmission block, there is one character which can be either MORE_PACKETS or END_OF_FILE.
 */

std::string_view id_server = "OLYMPUS-SER-V ";
std::string_view id_client = "OLYMPUS-CLI-V ";
std::string_view version_server = "1";
std::string_view version_client = "1";

void log_packet(bool sending, std::string_view packet, bool headers) {
    OPEN_LOG_AS(DBG_NETWORK, strm);
    strm << "----" << (sending ? "SENDING" : "RECEIVING") << "----\n";
    auto end = headers ? packet.size() - 1 : packet.size();
    for(uint i=0; i<end; i++) {
        if(i % 20 == 0) strm << '<' << i << '>';
        if(packet[i] < 32 and packet[i] != '\n') strm << '[' << static_cast<uint>(packet[i]) << ']';
        else strm << packet[i];
    }
    if(headers){
        switch (packet[packet.size() - 1]) {
        case Networker::MORE_PACKETS :
            strm << "\nMORE PACKETS"; break;
        case Networker::END_OF_FILE :
            strm << "\nEND OF FILE"; break;
        default:
            strm << "\nERROR: last character is [" << static_cast<uint>(packet[packet.size() - 1]) << "]";
        }
    }
    strm << "\n----END (" << packet.size() << "characters)----";
    CLOSE_LOG(strm);
}

void send(tcp::socket& socket, std::string_view message, bool headers) {
    log_packet(true, message, headers);
    auto result = socket.send( boost::asio::buffer( message ) );
    assert(result == message.size());
}

void Sender::send_chunks(bool headers) {
    // also include the write head, which points to the last character (END_OF_FILE or MORE_PACKETS)
    ::send(socket, std::string_view(buffer.data(), writehead + (headers ? 1 : 0)), headers );
    writehead = 0;
}

bool Sender::try_add_chunk(std::istream& ifile) {
    ifile.getline(buffer.begin() + writehead, buffer.size() - writehead - 1);
    //Writes max. BS - wh chars from position wh until position BUFFER_SIZE - 2
    //(BUFFER_SIZE - 1 is reserved for MORE_PACKETS / END_OF_FILE) 
    //When less than one line is extracted, the FAIL bit is set

    gdebug(DBG_NETWORK) << "Attempting to read " << buffer.size() - writehead - 1 << " chars, only "
              << ifile.gcount() << " read, '" << buffer.begin() + writehead
              << "', fail=" << ifile.fail() << ", eof=" << ifile.eof() << '\n';

    if(ifile.eof()) {
        //no '\n' was extracted
        writehead += ifile.gcount();
        buffer[writehead] = Networker::END_OF_FILE;
        return true; //eof encountered before fail -> success
    } else if(ifile.fail()) {
        if(writehead == 0){
            //std::cout << "Error: Buffer is too small to send one line.\n";
            exit(1);
        }
        else return false; //stay in a coherent state, return false and let the calling function retry
    } else {
        writehead += ifile.gcount();
        buffer[writehead-1] = '\n'; //getline() replaces \n by \0, which we don't want because we want to transmit the file 'as-is'.
        buffer[writehead] = Networker::MORE_PACKETS; //this is not out of scope because writing always stops at BUFFER_SIZE-2 or before.
        return true;
    }
}

void Sender::add_chunk_from_file(std::istream& ifile) {
    bool success = try_add_chunk(ifile);
    if(!success) {
        ::send( socket,std::string_view(buffer.data(), writehead), true ); //transmit each character up to writehead-1

        //the bytes from writehead to BUFFER_SIZE - 1 could not be transmitted and are copied to the start of the buffer
        for (uint i = writehead; i < buffer.size() - 1; ++i)
            buffer[i - writehead] = buffer[i];
        //writehead points right after these bits
        writehead = buffer.size() - 1 - writehead;
        ifile.clear(); //reset the "fail" bit

        assert( try_add_chunk(ifile) == true ); //we gave it a clear buffer so there's no reason it should fail this time
    }
}

void Sender::add_chunk(const char* msg, std::size_t len) {
    if(len > buffer.size() - 1 - writehead) send_chunks();
    if(len > buffer.size() - 1) exit(1);
    for(uint i = 0; i<len+1; i++) buffer[writehead + i] = msg[i];
    writehead += len;
    buffer[writehead] = Networker::MORE_PACKETS;
}

void Sender::close(bool headers) {
    buffer[writehead] = Networker::END_OF_FILE;
    send_chunks(headers);
}

void Networker::sendFile(std::istream& file) {
    Sender sender(socket);
    do { //sending all segments
        sender.add_chunk_from_file(file);
    } while(not file.eof());
    sender.close(true);
}

std::string Networker::receiveMessage_sync() {
    assert(socket.is_open());
    unsigned long gcount = socket.receive(boost::asio::buffer(buffer));
    log_packet(false, std::string_view(buffer.data(), gcount));
    return { buffer.data(), gcount };
}

std::string Networker::receiveLongMessage_sync() {
    std::stringstream ret;
    while(true) { //read all segments
        auto packet = receiveMessage_sync();
        ret.write(packet.data(), packet.size() - 1); //not including the END_OF_FILE
        if(packet.back() == END_OF_FILE) {
            return ret.str();
        }
        else assert(packet.back() == MORE_PACKETS);
    }
}

uptr<std::istream> Networker::receiveFile_sync() {
    return std::make_unique<std::stringstream>(receiveLongMessage_sync());
}
