#include "catch2/catch_test_macros.hpp"
#include "headE_enums.h"
#include "3player_board.h"
#include "gameplay/2cards.h"
#include <algorithm>

TEST_CASE("Yggdrasil", "[yggdrasil]") {
    BoardN board;

    REQUIRE(board.begin() == board.end());
    REQUIRE(board.pbegin() == board.pend());
    REQUIRE(board.mycreas.begin() == board.mycreas.end());
    REQUIRE(board.empty());
    REQUIRE(board.cpbegin() == board.cpend());

    card_type type; type.underlying = card_type::flagged; type.legendary = 1;
    auto orc = std::make_shared<CardOracle>(type);
    uptr<Card> card = std::make_unique<Card>(orc);
    board.insert(std::move(card), nullptr);

    REQUIRE_FALSE(board.myartos.getObject(0)->getChildren().empty());
    REQUIRE_FALSE(board.myartos.getObject(0)->empty());
    REQUIRE_FALSE(board.myartos.empty());
    REQUIRE_FALSE(board.myartos.begin() == board.myartos.end());
    REQUIRE_FALSE(board.begin() == board.end());
    REQUIRE_FALSE(board.pbegin() == board.pend());
    REQUIRE_FALSE(board.cpbegin() == board.cpend());
}
