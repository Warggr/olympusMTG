#include "agent.h"
//#include "bot/botagent.h"
//#include "local/localagent.h"
#include "network/networkagent.h"

std::unique_ptr<Agent> createAgent(playerType desc) {
    switch(desc) {
        //case LOCAL: return std::make_unique<LocalAgent>();
        //case BOT: return std::make_unique<BotAgent>();
        case NETWORK: return std::make_unique<NetworkAgent>();
    }
    return nullptr;
}
