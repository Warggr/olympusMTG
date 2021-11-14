#include "catch2/catch_test_macros.hpp"
#include "agent.h"
#include "mock/mockagent.h"
#include "bot/botagent.h"
#include "local/localagent.h"
#include "agents/network/networkagent.h"

TEST_CASE("Agents", "[agents]") {
    uptr<Agent> agent;
    agent = Agent::factory(playerType::BOT);
    REQUIRE(dynamic_cast<BotAgent*>(agent.get()) != nullptr);

    agent = Agent::factory(playerType::LOCAL);
    REQUIRE(dynamic_cast<LocalAgent*>(agent.get()) != nullptr);

    agent = Agent::factory(playerType::NETWORK);
    REQUIRE(dynamic_cast<NetworkAgent*>(agent.get()) != nullptr);
}
