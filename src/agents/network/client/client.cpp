#include "OlympusClient.h"
#include <iostream>

[[noreturn]] void printUsageAndExit() {
    std::cout << "Usage: olympus_client [HOST_IP_ADDR YOUR_NAME]\n";
    exit(1);
}

int main(int argc, char** argv) {
    OlympusClient client;
    if(argc == 3) client.init(argv[1], argv[2]);
    else if(argc == 4) client.init(argv[1], argv[2], argv[3]);
    else client.init();
    client.start();
}
