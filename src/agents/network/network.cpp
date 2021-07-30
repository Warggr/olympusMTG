#include "network.h"
#include <unistd.h>
#include <fstream>
#include <iostream>

const char* id_server = "OLYMPUS-SER-V ";
const char* id_client = "OLYMPUS-CLI-V ";
const char* version_server = "1";
const char* version_client = "1";

void Networker::send_file(std::ifstream& file) {
    std::cout << "Start send_file function\n";
    char buffer[BUFFER_SIZE];
    int start = 0; //points to the 1st location in memory that should be written to
    while(true) { //sending all segments
        file.getline(buffer + start, BUFFER_SIZE - start);

        if (file.fail() || BUFFER_SIZE == start) { //buffer limit reached
            if(start == 0){
                std::cout << "Error: Buffer is too small to send one line.\n";
                exit(1);
            }
            net_send(buffer, start);
            std::cout << "Sent one segment\n";
            //the last BUFFER_SIZE - start bits could not be transmitted and are copied to start of buffer
            for (int i = start; i < BUFFER_SIZE; i++)
                buffer[i - start] = buffer[i];
            //start points right after these bits
            start = BUFFER_SIZE - start;
        } else {
            start += file.gcount(); //advance start
            if (file.eof()) { //file completely transmitted
                buffer[start+1] = '\n'; //send \n as final character (since \0 already separates each line)
                net_send(buffer, start + 2);
                std::cout << "Send last segment\n";
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
    std::cout << message << "\n";
    if(n < 0) throw NetworkError();
}
