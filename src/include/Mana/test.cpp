#include "lib2_mana.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "Mana library", "[mana]" ) {
    Mana mana("1WWW");
    SECTION("CMC") {
        REQUIRE( mana.cmc() == 4 );
    }

    INFO(Mana::positions::WHITE);
    SECTION("Colors") {
        REQUIRE_FALSE( mana >= Mana("R"));
        REQUIRE( mana.m2color() == colorId::white );
        REQUIRE( mana.m2x()[Mana::positions::WHITE] == 3 );
    }

    mana -= Mana("1W");

    SECTION("Substraction") {
        std::string str = mana.m2t();
        REQUIRE( (str[0] == 'W' and str[1] == 'W') );
        REQUIRE( mana >= Mana("W") );
    }
}

TEST_CASE( "Zero mana", "[mana]") {
    Mana mana("3WWUR");
    mana -= mana;
    REQUIRE( mana.cmc() == 0);

    REQUIRE( mana.m2t().size() == 1 );
    REQUIRE( mana.m2t()[0] == '0' );
}
