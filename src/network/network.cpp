#include "network.h"
#include <istream>
#include <iostream>
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

const char* id_server = "OLYMPUS-SER-V ";
const char* id_client = "OLYMPUS-CLI-V ";
const char* version_server = "1" ;
const char* version_client = "1";

void Sender::send_chunks() {
    for(uint i=0; i<10 and i<writehead; i++){
        if(buffer[i] > 32) std::cout << buffer[i];
        else std::cout << '[' << static_cast<int>(buffer[i]) << ']';
    }
    std::cout << '\n';
    Networker::send(buffer, writehead + 1, sockfd); //writehead + 1 contains the MORE_PACKETS flag and is always in scope.
    writehead = 0;
}

bool Sender::try_add_chunk(std::istream& ifile) {
    ifile.getline(buffer + writehead, BUFFER_SIZE - writehead - 1);
    //Writes max. BS - wh chars from position wh until position BUFFER_SIZE - 2
    //(BUFFER_SIZE - 1 is reserved for MORE_PACKETS / END_OF_FILE) 
    //When less than one line is extracted, the FAIL bit is set

    std::cout << "Attempting to read " << BUFFER_SIZE - writehead - 1 << " chars, only "
              << ifile.gcount() << " read, '" << buffer + writehead
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

void Sender::add_chunk(std::istream& ifile) {
    bool success = try_add_chunk(ifile);
    if(!success) {
        Networker::send(buffer, writehead, sockfd); //transmit each character up to writehead-1

        //the bytes from writehead to BUFFER_SIZE - 1 could not be transmitted and are copied to the start of the buffer
        for (uint i = writehead; i < BUFFER_SIZE - 1; ++i)
            buffer[i - writehead] = buffer[i];
        //writehead points right after these bits
        writehead = BUFFER_SIZE - 1 - writehead;
        ifile.clear(); //reset the "fail" bit

        assert( try_add_chunk(ifile) == true ); //we gave it a clear buffer so there's no reason it should fail this time
    }
}

void Sender::add_chunk(const char* msg, unsigned int len) {
    if(len > BUFFER_SIZE - 1 - writehead) send_chunks();
    if(len > BUFFER_SIZE - 1) exit(1);
    for(uint i = 0; i<len+1; i++) buffer[writehead + i] = msg[i];
    writehead += len;
    buffer[writehead] = MORE_PACKETS;
}

void Sender::close() {
    std::cout << "Writing 1 to [" << writehead << "]\n";
    buffer[writehead] = END_OF_FILE;
    send_chunks();
}

void Networker::sendFile(std::istream& file) {
    Sender sender(sockfd);
    while(true) { //sending all segments
        sender.add_chunk(file);

        if(file.eof()) { //file completely transmitted
            sender.close();
            return;
        }
    }
}

const char* Networker::receiveMessage() {
    long n = ::read(sockfd, buffer, BUFFER_SIZE);
    std::cout << "----GOT MESSAGE----\n";
    for(int i=0; i<n; i++) {
        if(i % 5 == 0) std::cout <<  "[" << i << "]:";
        std::cout << (buffer[i] == 0 ? '_' : buffer[i]);
    }
    std::cout << "\n----END (" << n << "characters)----\n";
    if(n <= 0) throw NetworkError();
    buffer[n] = 0;
    return buffer;
}

Networker::~Networker() {
    if(connected) close(sockfd);
}

long Networker::send(const char* message, unsigned long length, int sockfd) {
    long n = write(sockfd, message, length);
    std::cout << "----SENDING----\n";
    for(unsigned int i=0; i<length+2; i++){
        if(i % 10 == 0) std::cout <<  "[" << i << "]:";
        std::cout << (message[i] == 0 ? '_' : message[i]);
    }
    std::cout << "\n----END (" << length << "characters)----\n";
    if(n < 0) throw NetworkError();
    return n;
}

uptr<std::istream> Networker::receiveFile() {
    auto ret = std::make_unique<std::stringstream>();
    while(true) { //read all segments
        long n = read();
        ret->write(buffer, n-1); //not including the 0 or the 1
        if(buffer[n-1] == Sender::END_OF_FILE) {
            return ret;
        }
    }
}

long Networker::read() {
    std::cout << "Waiting...\n";
    long message_length = ::read(sockfd, buffer, BUFFER_SIZE-1);
    if(message_length == -1){
        std::cout << "read failed: " << std::strerror(errno) << '\n';
        exit(1);
    }
    buffer[message_length] = 0;
    std::cout << "----RECEIVED READ----\n";
    for(int i=0; i<message_length; i++) {
        if(i % 10 == 0) std::cout <<  "[" << i << "]:";
        std::cout << (buffer[i] == 0 ? '_' : buffer[i] == 1 ? '#' : buffer[i]);
    }
    std::cout << "\n----END READ (" << message_length << "characters)----\n";
    if (message_length == 0) { //Somebody disconnected , get his details and print
        sockaddr_in cli_addr;
        socklen_t clilen = sizeof(cli_addr);
        getpeername(sockfd, (sockaddr*) &cli_addr, &clilen);
        //printf("Host disconnected , ip %s , port %d \n", inet_ntoa(address.sin_addr) , ntohs(address.sin_port));

        //Close the socket
        close( sockfd );
        connected = false;
    }
    return message_length;
}

void Networker::contact(const char* hostIp) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) throw NetworkError();

    in_addr addr;
    if(inet_aton(hostIp, &addr) < 0) throw NetworkError();

    struct sockaddr_in serv_addr = {
            .sin_family = AF_INET,
            .sin_port = htons(PORT_NO),
            .sin_addr = addr,
            .sin_zero = {0}
    };
    if (connect(sockfd, (sockaddr*) &serv_addr, sizeof(serv_addr)) < 0) throw NetworkError();
}
