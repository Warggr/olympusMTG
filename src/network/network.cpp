#include "network.h"
#include <istream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>

const char* id_server = "OLYMPUS-SER-V ";
const char* id_client = "OLYMPUS-CLI-V ";
const char* version_server = "1" ;
const char* version_client = "1";

void Networker::send_file(std::istream& file) {
    //std::cout << "Start send_file function\n";
    char outgoing_buffer[BUFFER_SIZE];
    int writehead = 0; //points to the 1st location in memory that should be written to
    while(true) { //sending all segments
        file.getline(outgoing_buffer + writehead, BUFFER_SIZE - writehead);
        std::cout << "Attempting to read " << BUFFER_SIZE - writehead << " chars, only "
        << file.gcount() << " read, '" << outgoing_buffer + writehead
        << "', fail=" << file.fail() << ", eof=" << file.eof() << '\n';
        //Puts a \0 at the end of the extracted characters instead of \n.
        //Will be overwritten by the next line.
        //When less than one line is extracted, the FAIL bit is set

        if (file.eof()) { //file completely transmitted
            for(int i=0; i<writehead + file.gcount() + 1; i++) {
                if(i % 5 == 0) std::cout <<  "[" << i << "]:";
                std::cout << (outgoing_buffer[i] == 0 ? '_' : outgoing_buffer[i]);
            }
            send(outgoing_buffer, writehead + file.gcount() + 1); //This time, also send the \0
            //std::cout << "Send last segment with size " << start+1 << "\n";
            return;
        } else if (file.fail()) { //if we don't have space left
            if(writehead == 0){
                //std::cout << "Error: Buffer is too small to send one line.\n";
                exit(1);
            }
            for(int i=0; i<writehead; i++) {
                if(i % 5 == 0) std::cout <<  "[" << i << "]:";
                std::cout << (outgoing_buffer[i] == 0 ? '_' : outgoing_buffer[i]);
            }
            std::cout << "\n";
            send(outgoing_buffer, writehead);

            //the last BUFFER_SIZE - start bits could not be transmitted and are copied to start of buffer
            for (int i = writehead; i < BUFFER_SIZE; i++)
                outgoing_buffer[i - writehead] = outgoing_buffer[i];
            //start points right after these bits
            writehead = BUFFER_SIZE - writehead;
            file.clear(); //reset the "fail" bit

            file.getline(outgoing_buffer + writehead, BUFFER_SIZE - writehead);
            writehead += file.gcount();
            if(file.fail()) {
                //std::cout << "Error: Buffer is too small to send one line.\n";
                exit(1);
            }
        } else {
            writehead += file.gcount();
            outgoing_buffer[writehead-1] = '\n';
        }
    }
}

const char* Networker::getMessage() {
    long n = ::read(sockfd, buffer, BUFFER_SIZE);
    if(n < 0) throw NetworkError();
    buffer[n] = 0;
    return buffer;
}

Networker::~Networker() {
    if(connected) close(sockfd);
}

void Networker::send(const char* message, unsigned long length) const {
    long n = write(sockfd, message, length);
    if(n < 0) throw NetworkError();
}

uptr<std::istream> Networker::receive_file() {
    auto ret = std::make_unique<std::stringstream>();
    while(true) { //read all segments
        long segment_length = ::read( sockfd , buffer, BUFFER_SIZE);
        if(segment_length == -1){
            std::cout << "read failed: " << std::strerror(errno) << '\n';
            exit(1);
        }
        else if(segment_length == 0) {
            std::cout << "Zero-length packet received?\n";
            continue;
        }
        for(int i=0; i<segment_length; i++) {
            if(i % 5 == 0) std::cout <<  "[" << i << "]:";
            std::cout << (buffer[i] == 0 ? '_' : buffer[i]);
        }
        assert(buffer[segment_length-1] == '\0' or buffer[segment_length-1] == '\n');
        std::cout << "\n";
        ret->write(buffer, segment_length - 1); //ignoring the last \0
        if(buffer[segment_length-1] == '\n') {
            return ret;
        }
    }
}

long Networker::read() {
    long message_length = ::read(sockfd, buffer, BUFFER_SIZE);
    buffer[message_length] = 0;
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
