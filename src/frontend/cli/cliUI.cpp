#include "lib3_cliUI.h"
#include "lib3_nanoIO.h"
#include "classes/perm_option.h"
#include "gameplay/2cards.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/resolvables/stack.h"
#include "control/3player.h"
#include "yggdrasil/iterators.h"

void CliUI::addCards(const std::list<CardWrapper>&) {}

void CliUI::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    (void) power; (void) blockers; //TODO
}

const Target* CliUI::chooseTarget(char type) {
    (void) type; return nullptr;
}

void CliUI::chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) {
    (void) defenders; (void) attackers;
}

void CliUI::disp(const fwdlist<card_ptr>& list, uint size) {
    auto iter = list.begin();
    for(uint i = 0; i < size; ++i, ++iter) {
        io.disp(**iter, NanoIO::INLINE);
    }
}

void CliUI::list(const Context& context, const Option::CastingContext& castingContext) {
    int i = 0;
    if(context.where == zone::stack){
        for (const auto& x: *Stack::god) io.disp_inrow(x.get(), i++, Stack::god->size(), BasicIO::INLINE);
    } else {
        for (const Gamer* player: context.players) {
            switch (context.where) {
                case zone::hand:
                    for (const auto& card: player->getHand())
                        io.disp_inrow(card.get(), i++, player->getHand().size(),
                                      BasicIO::INLINE | (card.isCastable(castingContext) ? BasicIO::HIGHLIGHT : 0));
                    break;
                case zone::battlefield:
                    for (auto perm = pl->myboard.cbegin(); perm != pl->myboard.end(); ++perm)
                        io.disp_inrow(&(*perm), i++, player->myboard.size(), BasicIO::INLINE);
                    break;
                case zone::exile:
                case zone::commandzone:
                case zone::graveyard: {
                    Player::myzone zn =
                            context.where == zone::exile ? Player::exile : context.where == zone::commandzone
                                                                           ? Player::command
                                                                           : Player::graveyard;
                    for (const auto& card: pl->getZone(zn).getCards())
                        io.disp_inrow(raw_cardptr(card), i++, player->getZone(zn).getSize(), BasicIO::INLINE);
                    break;
                }
                case zone::stack:
                    assert(false);
            }
        }
    }
}

void CliUI::registerMe(const Gamer* player) {
    pl = player;
}
