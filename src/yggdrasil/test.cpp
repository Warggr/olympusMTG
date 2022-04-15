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
    REQUIRE(board.size() == 0);
    REQUIRE(board.cpbegin() == board.cpend());

    Card card(nullptr);
    board.insert(move_cardptr(&card), nullptr);

    REQUIRE_FALSE(board.myartos.getObject(0)->getChildren().empty());
    REQUIRE_FALSE(board.myartos.getObject(0)->empty());
    REQUIRE_FALSE(board.myartos.empty());
    REQUIRE_FALSE(board.myartos.begin() == board.myartos.end());
    REQUIRE_FALSE(board.begin() == board.end());
    REQUIRE_FALSE(board.pbegin() == board.pend());
    REQUIRE_FALSE(board.cpbegin() == board.cpend());
    REQUIRE(board.size() == 1);
}
