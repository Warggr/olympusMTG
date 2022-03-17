#include "lib3_graphicalUI.h"

GraphicalUI::GraphicalUI() : io(AbstractIO::factory()) {
    io->setMenuScene();
    int screenW, screenH;
    io->getResolution(screenW, screenH, Screen::linesize);
    screen.initScreen(screenW, screenH);

    io->harmonize(screen.leftBar().poster().coords, screen.rightBar().messageZone().coords, 16); //nb windows
}

void GraphicalUI::registerPlayers(const std::list<const Gamer*>& players) {
    int i=0;
    for (auto iter = players.begin(); i < 2; i++, iter++) {
        screen.board().players()[i].header().setPlayer(*iter); //TODO FEATURE handle more than 2 players
    }
}

void GraphicalUI::addCards(const std::list<CardWrapper>& cards) {
    for(auto& card: cards){
        screen.rightBar().optionZone().insert(Sprite<CardWrapper>(&card));
    }
}

void GraphicalUI::splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) {
    for(auto& pair : blockers) {
        io->message(pair.second.getTarget()->getName());
        uint i = io->getInt(0, power);
        power -= i;
        pair.first = i;
    }
}

const Target* GraphicalUI::chooseTarget(char type) {
    return screen.iterate(type, true);
}

/*void GraphicalUI::chooseattackers(StateTN<Creature>& attackers) {
    int yOffset, zOffset;
    Rect rect = screen.board().players()[0].board().getCoordinates(&yOffset, &zOffset);
    for(auto& i : attackers) {
        bool attacks = io->attackSwitch(rect.y, rect.right(), rect.z, 15);
        if(attacks) {
            (void) i;//TODO actually make the creature attack
        }
    }
}*/

void GraphicalUI::chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) {
    int i = 0;
    for(Creature& defender : defenders) {
        Creature* attacker = blockerSwitch(defender, i, attackers);
        attacker->addBlocker(&defender);
        ++i;
    }
}

Creature* GraphicalUI::blockerSwitch(const Creature& blocker, int blockerIndex, StateTN<Creature>& attackers) {
    (void) blocker; (void) blockerIndex;
    return &(*attackers.begin()); //TODO
}

Option* GraphicalUI::chooseOpt(bool sorcerySpeed) {
    (void) sorcerySpeed;
    return nullptr; //TODO
}

void GraphicalUI::disp(fwdlist<uptr<Card>>::const_iterator begin, const fwdlist<uptr<Card>>::const_iterator end) {
    for(auto i = begin; i != end; ++i) {
        io->draw(*(*i)->oracle, Rect(0, 0, 0, 0), false); //TODO
    }
}

void GraphicalUI::registerMe(const Gamer* pl) {
    (void) pl;
}
