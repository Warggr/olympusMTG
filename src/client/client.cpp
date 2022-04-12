#include "OlympusClient.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

po::variables_map readArgs(int argc, char** argv) {
    po::options_description generic("Allowed options");
    generic.add_options()
            ("help,h", "produce help message")
            ("version,v", "print the version number")
            ("username,u", po::value<std::string>(), "player name")
            ("ip,i", po::value<std::string>(), "server IP address")
            ("filename,f", po::value<std::string>(), "deck file")
            ;

    po::positional_options_description p;
    p.add("username", 1);
    p.add("ip", 1);
    p.add("filename", 1);

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
            options(generic).positional(p).run(), vm);
    po::notify(vm);
    return vm;
}

void OlympusClient::init(const po::variables_map& vm) {
    playerName = vm.count("username") ? vm["username"].as<std::string>() : agent.getName();
    std::string hostIp = vm.count("ip") ? vm["ip"].as<std::string>() : agent.getLogin();

    std::thread net_thread(&NetworkClient::init, &network, hostIp.c_str(), playerName.c_str());
    //while we're waiting for the server to answer...

    std::unique_ptr<std::istream> file;
    if(vm.count("filename"))
        file = std::make_unique<std::ifstream>( vm["filename"].as<std::string>(), std::ios::in );
    else
        file = agent.getDeckFile(); //...we ask the player for his deck
    net_thread.join();
    network.sendFile(*file); //once both are done, we send the file
}

int main(int argc, char** argv) {
    OlympusClient client;
    auto vm = readArgs(argc, argv);
    client.init(vm);
    client.start();
}
