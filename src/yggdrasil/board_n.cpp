#include "3player_board.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/2cards.h"

void BoardN::insert(std::unique_ptr<Card> to_add, Player *pl) {
    switch(to_add->get_type().underlying) {
        case card_type::planeswalker:
            mysuperfriends.insert(std::move(to_add), pl); break;
        case card_type::creature:
            mycreas.insert(std::move(to_add), pl); break;
        default:
            myartos.insert(std::move(to_add), pl);
    }
}

void BoardN::remove(Permanent *perm) {
    switch(perm->getType()) {
        case permanent_type::creature: remove(static_cast<Creature*>(perm)); break;
        case permanent_type::planeswalker: remove(static_cast<Planeswalker*>(perm)); break;
        default: myartos.remove(perm); break;
    }
}

void BoardN::remove(Creature* crea) { mycreas.remove(crea); }
void BoardN::remove(Planeswalker* plan) { mysuperfriends.remove(plan); }
