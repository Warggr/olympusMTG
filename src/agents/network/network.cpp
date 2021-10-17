#include "network.h"
#include <unistd.h>
#include <istream>
#include <iostream>
#include <sstream>
#include <cassert>

const char* id_server = "OLYMPUS-SER-V ";
const char* id_client = "OLYMPUS-CLI-V ";
const char* version_server = "1";
const char* version_client = "1";

void Networker::send_file(std::istream& file) {
    //std::cout << "Start send_file function\n";
    char outgoing_buffer[BUFFER_SIZE];
    int start = 0; //points to the 1st location in memory that should be written to
    for(int no_inf_loop = 0; no_inf_loop < 100; no_inf_loop++) { //sending all segments
        file.getline(outgoing_buffer + start, BUFFER_SIZE - start);
        //Puts a \0 at the end of the extracted characters after \n, so at buffer[start].
        //Will be overwritten by the next line.
        //When less than one line is extracted, the FAIL bit is set

        if (file.fail() or BUFFER_SIZE == start) { //if we don't have space left
            if(start == 0){
                //std::cout << "Error: Buffer is too small to send one line.\n";
                exit(1);
            }
            net_send(outgoing_buffer, start);
            //std::cout << "Sent one segment with size " << start << "\n";
            //the last BUFFER_SIZE - start bits could not be transmitted and are copied to start of buffer
            for (int i = start; i < BUFFER_SIZE; i++)
                outgoing_buffer[i - start] = outgoing_buffer[i];
            //start points right after these bits
            start = BUFFER_SIZE - start;
            file.clear(); //reset the "fail" bit
        } else {
            start += file.gcount(); //advance start
            if (file.eof()) { //file completely transmitted
                net_send(outgoing_buffer, start + 1); //This time, also send the \0
                //std::cout << "Send last segment with size " << start+1 << "\n";
                return;
            }
        }
    }
}

const char* Networker::net_receive() {
    long n = read(sockfd, buffer, BUFFER_SIZE);
    if(n < 0) throw NetworkError();
    buffer[n] = 0;
    return buffer;
}

long Networker::receive() {
    return read(sockfd, buffer, BUFFER_SIZE);
}

Networker::~Networker() {
    if(connected) close(sockfd);
}

void Networker::net_send(const char* message, unsigned long length) const {
    long n = write(sockfd, message, length);
    if(n < 0) throw NetworkError();
}

uptr<std::istream> Networker::receive_file() {
    auto ret = std::make_unique<std::stringstream>();
    while(true) { //read all segments
        uint segment_length = read( sockfd , buffer, 1024);
        assert(buffer[segment_length-1] == '\0');
        //for(uint i=0; i<segment_length; i++) std::cout << "[" << i << "]: " << (buffer[i] == 0 ? '#' : buffer[i]) << "\n";
        ret->write(buffer, segment_length - 2); //ignoring the last \0
        if(buffer[segment_length-1] == '\n') {
            return ret;
        }
    }
}
