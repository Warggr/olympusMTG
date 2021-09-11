#include "catch2/catch_test_macros.hpp"
class Card {};

#include "3player_board.h"
#include <algorithm>

TEST_CASE("Yggdrasil", "[yggdrasil]") {
    BoardN board;
    auto x = board.begin();
    REQUIRE(x == board.end());
    REQUIRE(board.pbegin() == board.pend());
    REQUIRE(board.cpbegin() == board.cpend());

    uptr<Card> card = std::make_unique<Card>();
    board.insert(std::move(card), nullptr);
    REQUIRE_FALSE(board.begin() == board.end());
    REQUIRE_FALSE(board.pbegin() == board.pend());
    REQUIRE_FALSE(board.cpbegin() == board.cpend());
}
