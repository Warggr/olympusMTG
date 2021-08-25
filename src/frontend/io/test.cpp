#include "lib3_IO.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "I/O", "[io]" ) {
    ImplementIO myIO;
    std::string shouldBeOne = myIO.getTextInput("Please enter '1'");
    REQUIRE( (shouldBeOne.size() == 1 && shouldBeOne[0] == '1') );
    REQUIRE( myIO.simpleChoice("Please choose this option", "Don't choose this") == true);
}
