#include "OlympusClient.h"
#include <iostream>
#include <fstream>
#include <thread>
#include <boost/program_options.hpp>
#include <cstdlib>

namespace po = boost::program_options;

constexpr unsigned int default_port = 4242;

po::variables_map readArgs(int argc, char** argv) {
    po::options_description generic("Allowed options");
    generic.add_options()
            ("help,h", "produce help message")
            ("version,v", "print the version number")
            ("username,u", po::value<std::string>(), "player name")
            ("ip,i", po::value<std::string>(), "server IP address")
            ("port,p", po::value<unsigned int>()->default_value(default_port), "server port")
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

    if(vm.count("help")){
        std::cout << "Usage: " << argv[0] << " [username] [ip] [filename] \n";
        std::cout << "Flags: \n";
        std::cout << generic << '\n';
        exit(EXIT_SUCCESS);
    }

    return vm;
}

void OlympusClient::init(const po::variables_map& vm) {
    playerName = vm.count("username") ? vm["username"].as<std::string>() : agent.getName();
    std::string ip = vm.count("ip") ? vm["ip"].as<std::string>() : agent.getLogin();
    auto port = vm["port"].as<unsigned int>();

    auto endpoint = tcp::endpoint(ip::address::from_string(ip), port);

    std::thread net_thread(&NetworkClient::init, &network, endpoint, playerName.c_str());
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
    auto vm = readArgs(argc, argv);

    OlympusClient client;
    client.init(vm);
    client.start();
}
