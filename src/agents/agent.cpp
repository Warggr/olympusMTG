#include "agent.h"
#include "logging.h"
#include <cassert>

#ifdef MOCK_AGENT
#include "mock/mockagent.h"
#else
#include "bot/botagent.h"
#include "local/localagent.h"
#include "agents/network/networkagent.h"
#endif

uptr<Agent> Agent::factory(playerType desc) {
    switch(desc) {
        ENABLE_IF_MOCK(case MOCK: return std::make_unique<MockAgent>());
        ENABLE_IF_LOCAL(case LOCAL: return std::make_unique<LocalAgent>());
        ENABLE_IF_NETWORK(case NETWORK: return std::make_unique<NetworkAgent>());
        ENABLE_IF_BOT(case BOT: return std::make_unique<BotAgent>());
    }
    exit(1); //Shouldn't happen, dunno why GCC doesn't see this
}

std::vector<OracleDescr> Agent::getDeck() {
    std::vector<OracleDescr> ret;
    auto fb = getDeckFile();
    while(fb->peek() != fb->eof() and !fb->fail()) {
        OPEN_LOG_AS(DBG_READFILE, strm);
        char buffer[1024]; int nb;
        *fb >> nb >> std::skipws;
        strm << "Read " << nb << "\n";
        fb->getline(buffer, 1024);
        int read_str_size = fb->gcount();
        strm << "'";
        for(int i=0; i<read_str_size + 2; ++i) strm << (buffer[i] ? buffer[i] : '#');
        strm << "'\n";
        CLOSE_LOG(strm);

        assert(buffer[0] == ' ');
        ret.emplace_back(buffer[1] == '"' ? customcard : reference,
                         nb, std::string(buffer + 1));
        gdebug(DBG_READFILE) << "Peek: '" << fb->peek() << "'; Fail: " << fb->fail() << "\n";
    }
    return ret;
}

std::ostream& operator<<(std::ostream &output, const OracleDescr& desc) {
    output << (int) desc.nb << " ";
    switch(desc.type) {
        case reference: output << " (ref) "; break;
        case customcard: output << " (custom) "; break;
        case compiled_customcard: output << " (compiled) "; break;
    }
    output << desc.initializer;
    return output;
}
