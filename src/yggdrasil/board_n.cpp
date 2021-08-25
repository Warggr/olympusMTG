#include "3player_board.h"
#include "gameplay/2cards.h"

void BoardN::insert(std::unique_ptr<Card> to_add, Player *pl) {
    switch(to_add->get_type().underlying) {
        case card_type::planeswalker:
            mysuperfriends.insert(std::move(to_add), pl); break;
        case card_type::creature:
            mycreas.insert(std::move(to_add), pl); break;
        case card_type::basic:
            mylands.insert(std::move(to_add), pl); break;
        default:
            if(to_add->get_type().land)
                mylands.insert(std::move(to_add), pl);
            else
                myartos.insert(std::move(to_add), pl);
    }
}

/*void BoardN::remove(Permanent *perm, permanent_type type) {
    switch(type) {
        case permanent_type::land: mylands.remove(dynamic_cast<Land*>(perm)); break;
        case permanent_type::artifact: myartos.remove(dynamic_cast<Artifact*>(perm)); break;
        case permanent_type::planeswalker: mysuperfriends.remove(dynamic_cast<Planeswalker*>(perm)); break;
        case permanent_type::creature: mycreas.remove(dynamic_cast<Creature*>(perm)); break;
    }
}*/

void BoardN::remove(Creature *crea) { mycreas.remove(crea); }
void BoardN::remove(Land *land) { mylands.remove(land); }
void BoardN::remove(Artifact *arti) { myartos.remove(arti); }
void BoardN::remove(Planeswalker* plan) { mysuperfriends.remove(plan); }
