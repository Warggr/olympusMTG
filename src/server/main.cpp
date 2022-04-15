#include "server.h"
#include "build_types.h" //for program version
#include "binaryvisitor.h" //for canaries
#include "network/protocol.h" //for server version
#include <iostream>
#include <filesystem>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

struct Usage {
    bool refresh_db{false}, start_game{false};
    ENABLE_IF_LOCAL(uint local{0};)
    ENABLE_IF_BOT(uint bot{0};)
    ENABLE_IF_NETWORK(uint network{0};)
};

void printVersion();

Usage parseArgs(int nbargs, char** args) {
    Usage ret;
    po::options_description usage("Usage");
    usage.add_options()
            ("help,h", "produce help message")
            ("version,v", "print the version number")
            ("refresh-database,r", "refresh the oracles database")
            ENABLE_IF_BOT(("bot,b", po::value<uint>(&ret.bot)->default_value(0)->implicit_value(1), "number of bots"))
            ENABLE_IF_LOCAL(("local,l", po::value<uint>(&ret.local)->default_value(0)->implicit_value(1), "number of server-local players"))
            ENABLE_IF_NETWORK(("network,n", po::value<uint>(&ret.network)->default_value(0)->implicit_value(1), "number of players connected via network"))
            ;

    po::variables_map vm;
    po::store(po::command_line_parser(nbargs, args).options(usage).run(), vm);
    po::notify(vm);
    uint nb_players =
            ENABLE_IF_LOCAL(+ vm["local"].as<uint>())
            ENABLE_IF_BOT( + vm["bot"].as<uint>())
            ENABLE_IF_NETWORK(+ vm["network"].as<uint>());

    if(vm.contains("refresh-database")) ret.refresh_db = true;

    if(vm.contains("help")) std::cout << usage << '\n';
    else if(vm.contains("version")) printVersion();
    else if(nb_players == 0) { if(!ret.refresh_db) std::cout << usage << '\n'; }
    else if(nb_players < 2) std::cout << usage << '\n';
    else ret.start_game = true;
    return ret;
}

int main(int nbargs, char** args) {
    std::filesystem::current_path("../material");

    Usage usage = parseArgs(nbargs, args);
    if(usage.refresh_db) refreshDatabase();
    if(!usage.start_game) return 0;

    std::list<playerType> types;
    ENABLE_IF_LOCAL(for(uint j=0; j<usage.local; j++) types.push_front(LOCAL); )
    ENABLE_IF_NETWORK(for(uint j=0; j<usage.network; j++) types.push_front(NETWORK); )
    ENABLE_IF_BOT(for(uint j=0; j<usage.bot; j++) types.push_front(BOT); )

    Server server;
    server.addPlayers(types);
    server.launchGame();

    return 0;
}

void printVersion() {
    std::cout << "Olympus v" << Olympus_VERSION << '\n';
    std::cout << "Built " << __DATE__ << ", " << __TIME__ << '\n';
    std::cout << "Server protocol " << id_server << version_server << '\n';
#ifdef F_CANARY
    std::cout << "Built with canaries\n";
#endif
}
