#include "reader.h"
#include "gameplay/2cards.h"
#include "classes/card_oracle.h"
#include "headE_enums.h"
#include "visit.hpp"
#include "reader.h"
#include "writer.h"

void CardOracle::init(ReaderVisitor& reader) {
    visit<true>(*this, reader);
    if(type.land) color = colorId::colorless;
    else color = rules.cost.mana.m2color();
}

void Card::write(WriterVisitor& writer) const {
    visit<false>(*oracle, writer);
}

std::string Card::toStr(const CardOracle* begin) const {
    uint16_t offset = oracle - begin;
    return std::string().assign(reinterpret_cast<const char*>(&offset), sizeof(offset));
}

void Card::fromStr(std::istream& ifile, const CardOracle* begin) {
    uint16_t offset; ifile.read(reinterpret_cast<char*>(&offset), sizeof(offset));
    oracle = begin + offset;
}

Effect_H::Effect_H(ReaderVisitor& reader){ visit<true>(*this, reader); }

namespace target_type {
    const flag_t tars1[] = { damageable, nonnegative };
    const flag_t tars2[] = { player, nonnegative };
    const flag_t tars3[] = { player, number };
    const flag_t tars4[] = { player, added_mana };
    const flag_t tars5[] = { permanent };
    const flag_t tars6[] = { resolvable };
    const flag_t tars7[] = { permanent, strictpositive };
    const flag_t tars8[] = { permanent };
    const flag_t* target_types[] = { tars1, tars2, tars3, tars3, tars4, tars5, tars5, tars6, tars7, tars8 };
    const int target_numbers[] = { 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1 };
}

void ReaderVisitor::readSubtypes() { }
