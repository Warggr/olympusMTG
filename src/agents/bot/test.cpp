#include "catch2/catch_test_macros.hpp"
#include "botagent.h"

TEST_CASE("Bot Agent", "[agent][bot]"){
    BotAgent botAgent;
    for(uint i=0; i<10; ++i) {
        REQUIRE(botAgent.chooseAmong(std::vector<PermOption*>(i)) < i);
    }
}
