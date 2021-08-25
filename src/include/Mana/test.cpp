#include "lib2_mana.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "Mana library", "[mana]" ) {
    Mana mana("1WWW");
    REQUIRE( mana.cmc() == 4 );
    mana -= Mana("1W");
    REQUIRE( (mana.m2t().c_str()[0] == 'W' &&  mana.m2t().c_str()[1] == 'W') );
    REQUIRE( mana >= Mana("W") );
    INFO(mana.m2t());
    INFO((int) mana.m2color().fields);
    REQUIRE(mana.m2color() == colorId::white );
    REQUIRE( mana.m2x()[Mana::positions::WHITE] == 2 );
}
