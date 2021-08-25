#include "agent.h"
#include "bot/botagent.h"
#include "local/localagent.h"
#include "network/networkagent.h"

std::unique_ptr<Agent> createAgent(playerType desc) {
    switch(desc) {
#ifdef MOCK_AGENT
        case MOCK: return std::make_unique<MockAgent>();
#else
        case LOCAL: return std::make_unique<LocalAgent>();
        //case BOT: return std::make_unique<BotAgent>();
        //case NETWORK: return std::make_unique<NetworkAgent>();
#endif
    }
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
