#include "lib3_graphicalUI.h"
#include "windows/window.h"

GraphicalUI::GraphicalUI() : io(AbstractIO::factory()) {
    io->setMenuScene();
    UIElement::io = io;

    int screenW, screenH;
    io->getResolution(screenW, screenH, Screen::linesize);
    screen.initScreen(screenW, screenH);

    io->harmonize(screen.leftBar().poster().getCoords(), screen.rightBar().messageZone().getCoords(), 16); //nb windows
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

const Option* GraphicalUI::chooseOpt(const Option::CastingContext& context) {
    (void) context;
    return nullptr; //TODO
}

void GraphicalUI::disp(const fwdlist<card_ptr>& lib, uint size) {
    uint total_size = screen.rightBar().getCoords().left();
    uint stepsize = total_size / size;
    Rect rect(0, 0, 375, 523); //TODO IMPLEM put actual IO-independant coordinates
    auto iter = lib.begin();
    for(uint i = 0; i < size; ++i, ++iter) {
        io->draw(*(*iter)->oracle, rect, false);
        rect.shift(stepsize, 0);
    }
}

void GraphicalUI::registerMe(const Gamer* pl) {
    (void) pl;
}
