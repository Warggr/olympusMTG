//#include "catch2/catch_test_macros.hpp"
#include "dictholder.h"

#define TEST_CASE(a, b) int main()
#define REQUIRE(b) if(!(b)) std::cout << "error"

TEST_CASE("Dictionary", "[dict]") {
    DictHolder dicts;
    REQUIRE(*dicts.dict_trigtypes.find("etb") == trigtype::etb);
    REQUIRE(dicts.dict_trigtypes.find("imaginary") == dicts.dict_trigtypes.not_found);

    REQUIRE(*dicts.dict_selector_players.find("you") == 0);
    REQUIRE(dicts.dict_selector_tribes.find("alien") == dicts.dict_selector_tribes.not_found);
}
