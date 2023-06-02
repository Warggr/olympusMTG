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

static constexpr char MORE_PACKETS = '\n', END_OF_FILE = '\0';
static_assert(MORE_PACKETS != END_OF_FILE);

std::string_view id_server = "OLYMPUS-SER-V ";
std::string_view id_client = "OLYMPUS-CLI-V ";
std::string_view version_server = "1" ;
std::string_view version_client = "1";

void Sender::send_chunks() {
    socket.send(boost::asio::buffer(buffer) );
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
        //The position of writehead doesn't matter anymore and no '\n' was extracted
        buffer[writehead + ifile.gcount()] = END_OF_FILE;
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
        buffer[writehead] = MORE_PACKETS; //this is not out of scope because writing always stops at BUFFER_SIZE-2 or before.
        return true;
    }
}

void Sender::add_chunk_from_file(std::istream& ifile) {
    bool success = try_add_chunk(ifile);
    if(!success) {
        socket.send( boost::asio::buffer(buffer, writehead) ); //transmit each character up to writehead-1

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
    buffer[writehead] = MORE_PACKETS;
}

void Sender::close() {
    buffer[writehead] = END_OF_FILE;
    send_chunks();
}

void Networker::send(std::string_view message) {
    auto result = socket.send( boost::asio::buffer( message ) );
    assert(result == message.size());

    OPEN_LOG_AS(DBG_NETWORK, strm);
    strm << "----SENDING----\n";
    for(uint i=0; i<message.size(); i++) {
        if(i % 20 == 0) strm << '<' << i << '>';
        if(message[i] < 32 and message[i] != '\n') strm << '[' << static_cast<uint>(message[i]) << ']';
        else strm << message[i];
    }
    strm << "\n----END (" << message.size() << "characters)----\n";
    CLOSE_LOG(strm);
}

void Networker::sendFile(std::istream& file) {
    Sender sender(socket);
    while(true) { //sending all segments
        sender.add_chunk_from_file(file);

        if(file.eof()) { //file completely transmitted
            sender.close();
            return;
        }
    }
}

void Networker::await_packet() {
    gcount = socket.receive(boost::asio::buffer(buffer));

    OPEN_LOG_AS(DBG_NETWORK, strm);
    strm << "----GOT MESSAGE----\n";
    for(int i=0; i<gcount; i++) {
        if(i % 20 == 0) strm <<  '<' << i << ">";
        if(buffer[i] < 32 and buffer[i] != '\n') strm << '[' << static_cast<uint>(buffer[i]) << ']';
        else strm << buffer[i];
    }
    strm << "\n----END (" << gcount << "characters)----\n";
    CLOSE_LOG(strm);
}

std::string Networker::receiveMessage_sync() {
    await_packet();
    assert(buffer[gcount] == END_OF_FILE);
    return std::string( &buffer.front(), gcount - 1 ); // trim the END_OF_FILE
}

uptr<std::istream> Networker::receiveFile_sync() {
    auto ret = std::make_unique<std::stringstream>();
    while(true) { //read all segments
        await_packet();
        ret->write(&buffer.front(), gcount-1); //not including the END_OF_FILE
        if(buffer[gcount] == END_OF_FILE) {
            return ret;
        }
        else assert(buffer[gcount] == MORE_PACKETS);
    }
}
