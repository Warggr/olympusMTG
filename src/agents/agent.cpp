#include "agent.h"
#include <cassert>

#ifdef MOCK_AGENT
#include "mock/MockAgent.h"
#else
#include "bot/botagent.h"
#include "local/localagent.h"
#include "network/networkagent.h"
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
    while(fb->peek() != EOF) {
        char buffer[1024]; int nb;
        *fb >> nb >> std::skipws;
        fb->getline(buffer, 1024);
        assert(buffer[0] == ' ');
        ret.emplace_back(buffer[1] == '"' ? customcard : reference,
                         nb, std::string(buffer + 1));
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
