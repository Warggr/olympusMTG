#include "3player_board.h"
#include "master_n.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"

void BoardN::insert(uptr<Card> to_add, Player* pl) {
    src::logger& lg = dbg_yggdrasil::get();
    logging::record rec = lg.open_record();
    logging::record_ostream strm(rec);

    disp(0, strm);
    strm.flush();
    lg.push_record(boost::move(rec));

    switch(to_add->getType().underlying) {
        case card_type::planeswalker:
            mysuperfriends.insert(std::move(to_add), pl); break;
        case card_type::creature:
            mycreas.insert(std::move(to_add), pl); break;
        default:
            myartos.insert(std::move(to_add), pl);
    }

    rec = lg.open_record();
    logging::record_ostream strm2(rec);
    disp(0, strm2);
    strm2.flush();
    lg.push_record(boost::move(rec));
}

void BoardN::remove(Permanent* perm) {
    switch(perm->getType()) {
        case permanent_type::creature: remove(static_cast<Creature*>(perm)); break;
        case permanent_type::planeswalker: remove(static_cast<Planeswalker*>(perm)); break;
        default: myartos.remove(perm); break;
    }
}

void BoardN::remove(Creature* crea) { mycreas.remove(crea); }
void BoardN::remove(Planeswalker* plan) { mysuperfriends.remove(plan); }

void MasterN::disp(unsigned int indent, logging::record_ostream& strm) const {
    for(uint i = 0; i<indent; i++) strm << ' ';
    strm << "---Full Board @" << this << '\n';
    for(auto& i : boards) i->disp(indent + 1, strm);
}
