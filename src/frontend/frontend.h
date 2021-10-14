#ifndef OLYMPUS_FRONTEND_H
#define OLYMPUS_FRONTEND_H

#include "lib3_IO.h"
#include "ui/12defaultUI.h"
#include "oracles/classes/card_oracle.h"
#include <string>
#include <list>

/* FrontEnd recreates part of the game state on its own.
 * What it has:
 * the printed cards, the changed cards,
 * What it doesn't have:
 * most hidden information
 */

//FrontEnd acts as a gateway between the program code and the local IO/UI.
// Every call to FrontEnd will switch the context and update the AbstractIO::god and AbstractUI::god values.

struct FrontEnd {
    ImplementIO io;
    ImplementUI ui;
    CardOracle* mycards;

    FrontEnd(): io(), ui(&io) { io.setMenuScene(); }
    inline void switchContext() { AbstractIO::god = &io; }
    std::string getName();
    std::string getLogin();
    std::ifstream getDeck();

//    void create(const char* descr);
//    void update(const char* descr);
//    void del(const char* descr);
//    void bulkOp(const char* descr);
    bool askMulligan();

    void splitDamage(int power, std::__cxx11::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers);

    uptr<OptionAction> chooseOpt(bool sorcerySpeed) { return ui.chooseOpt(sorcerySpeed); }
};

#endif //OLYMPUS_FRONTEND_H
