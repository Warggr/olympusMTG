#include "catch2/catch_test_macros.hpp"
#include "classes/card_oracle.h"
#include "filereader/plain/filereader.h"
#include "filereader/plain/Dictionary/dictholder.h"
#include <sstream>

TEST_CASE("Oracles") {
    SECTION("Lightning Bolt"){
        DictHolder dicts;

        std::string bolt_str = "<Lightning Bolt> I R { ping %any 3. <x>Fight fire with fire. }";
        std::stringstream bolt_stream(bolt_str);
        PlainFileReader reader(&dicts, bolt_stream);

        CardOracle oracle;
        oracle.init(reader);

        REQUIRE(oracle.getType().toString() == "Instant");

        int power, toughness, frametype;
        auto allText = oracle.allText(power, toughness, frametype);

        REQUIRE(frametype == 0);
        REQUIRE(allText.size() == 2);
        REQUIRE(allText[0] == "Lightning Bolt deals 3 damage to any target.");
        REQUIRE(allText[1] == "Fight fire with fire.");
    }
}
