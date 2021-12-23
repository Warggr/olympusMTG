#include "catch2/catch_test_macros.hpp"
#include "lib3_ncursesIO.h"

TEST_CASE("Start", "[ncurses]"){
    NcursesIO io;
    io.message("Hello, world!");
    sleep(3);
    REQUIRE(io.simpleChoice("Click here!", "Do not click here!"));
}
