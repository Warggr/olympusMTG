#include "oracles/classes/card_oracle.h"
#include "oracles/filereader/visitor.h"
#include "catch2/catch_test_macros.hpp"
#include <sstream>

class FakeReader: public ReaderVisitor {
    void raise_error(const std::string& message) override { (void) message; throw std::exception(); }
};

TEST_CASE("Storm Crow", "[read-oracles]") {
    DictHolder dicts;
    std::stringstream crow("4 <Storm Crow> 1U {<k>Flying} 1/2");
    FakeReader reader;
    CardOracle oracle(visitor);
    REQUIRE(oracle.getName() == "Storm Crow");
    REQUIRE(oracle.getCost().mana == Mana("1U"));
}
