#include "12defaultUI.h"
#include "mockIO.h"
#include "catch2/catch_test_macros.hpp"

TEST_CASE( "UI" ) {
    MockIO myIO;
    DefaultUI defaultUi(&myIO);
}
