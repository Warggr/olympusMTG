#include <oracles/filereader/filereader.h>
#include "2cards.h"
#include <sstream>
#include <gameplay/permanents/4permanents.h>

int main() {
    DictHolder dicts;
    std::string mydescr = "hello world";
    std::stringstream mystream(mydescr);
    PlainFileReader visitor(&dicts, mystream);
    std::shared_ptr<CardOracle> oracle = std::make_shared<CardOracle>(visitor);
    auto card = std::make_unique<Card>(oracle);

    SpellOption option(card->get_type());
    SpellOption option2(true, false);
    option2.cast_opt(nullptr, card);

    Land land(std::move(card), nullptr);
}
