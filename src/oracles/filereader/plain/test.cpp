#include <sstream>
#include "filereader.h"
#include "Mana/lib2_mana.h"
#include "Dictionary/dictholder.h"
#include "classes/card_oracle.h"
#include "catch2/catch_test_macros.hpp"

using namespace std;

TEST_CASE( "Plain file reader", "[reader]" ) {
    DictHolder dicts;

    string strs[] = {
        "<Island> BL +Island { T::add you U. }",
        "<Ismaru, Hound of Konda> C +Hound W {<x>Arigato gosaimasu} 2/2",
        "<Llanowar Elves> C +Elf+Druid G { T::add you G. } 1/1",
        "<Griselbrand> LC +Demon 5BBB { pay }",
        "<Colossal Dreadmaw> C +Dinosaur 4GG {<k>Trample <x>" \
        "If you see the ground quake, run. If you hear its bellow, flee. When you see its teeth, it's already too late.} 6/6"
    };
    constexpr int NB_TEST_CASES = sizeof strs / sizeof strs[0];
    Mana costs[NB_TEST_CASES] = {"0", "W", "G", "5BBB", "4GG"};

    vector<CardOracle> oracles;
    for(const string& str : strs) {
        stringstream stream(str);
        PlainFileReader reader(&dicts, stream);
        oracles.emplace_back(reader);
    }

    SECTION("Mana cost") {
        for(int i=0; i<NB_TEST_CASES; i++) {
            REQUIRE(oracles[i].getCost().mana == costs[i]);
        }
    }
}
