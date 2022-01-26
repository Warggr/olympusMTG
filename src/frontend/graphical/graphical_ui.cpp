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

void GraphicalUI::registerPlayers(std::list<Player>& players) {
    int i=0;
    for (auto iter = players.begin(); i < 2; i++, iter++) {
        screen.board().players()[i].header().setPlayer(&(*iter));
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

Target* GraphicalUI::chooseTarget(char type) {
    return screen.iterate(type, true);
}

bool GraphicalUI::chooseattackers(Y_Hashtable<Creature>& cowards) {
    bool ret = false;
    int yOffset, zOffset;
    Rect rect = screen.board().players()[0].board().getCoordinates(&yOffset, &zOffset);
    for(auto& i : cowards) {
        bool attacks = io->attackSwitch(rect.y, rect.right(), rect.z, 15);
        if(attacks) {
            ret = true;
            (void) i;//TODO actually make the creature attack
        }
    }
    return ret;
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

Option* GraphicalUI::chooseOpt(bool sorcerySpeed) {
    (void) sorcerySpeed;
    return nullptr; //TODO
}

bool GraphicalUI::attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const {
    return io->attackSwitch(leftY, rightY, topZ, arrowlength);
}

void GraphicalUI::disp(fwdlist<uptr<Card>>::const_iterator begin, const fwdlist<uptr<Card>>::const_iterator end) {
    Rect rect(0, 0, 375, 523); //TODO IMPLEM put actual IO-independant coordinates
    for(auto i = begin; i != end; ++i) {
        io->draw(*(*i)->oracle, rect, false);
        rect.shift(380, 0);
        if(rect.right() > screen.getCoords().right()) break;
    }
}

void GraphicalUI::registerMe(Player* pl) {
    (void) pl;
}
