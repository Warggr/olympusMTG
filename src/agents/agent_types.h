#ifndef OLYMPUS_AGENT_CONTEXT_H
#define OLYMPUS_AGENT_CONTEXT_H

#include "agent.h"
#include <memory>

enum PlayerType {
#ifdef MOCK_AGENT
    #define ENABLE_IF_MOCK(x) x
    #define ENABLE_IF_LOCAL(x)
    #define ENABLE_IF_BOT(x)
    #define ENABLE_IF_NETWORK(x)
    MOCK,
#else
    #define ENABLE_IF_MOCK(x)
    #ifndef ENABLE_IF_LOCAL
        #define ENABLE_IF_LOCAL(x) x
        LOCAL,
    #endif
    #ifndef ENABLE_IF_BOT
        #define ENABLE_IF_BOT(x) x
        BOT,
    #endif
    #ifndef ENABLE_IF_NETWORK
        #define ENABLE_IF_NETWORK(x) x
        NETWORK,
    #endif
#endif
};

ENABLE_IF_NETWORK(class NetworkManager;)

struct AgentContext {
    ENABLE_IF_NETWORK(NetworkManager& server;)
};

std::unique_ptr<Agent> agent_factory(PlayerType desc, AgentContext& context);

#endif //OLYMPUS_AGENT_CONTEXT_H
