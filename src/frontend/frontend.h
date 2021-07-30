#ifndef OLYMPUS_FRONTEND_H
#define OLYMPUS_FRONTEND_H

#include "lib3_IO.h"
#include "ui/12defaultUI.h"
#include "oracles/classes/2cards.h"
#include <string>

/* FrontEnd recreates part of the game state on its own.
 * What it has:
 * the printed cards, the changed cards,
 * What it doesn't have:
 * most hidden information
 */

class FrontEnd {
    ImplementIO io;
    ImplementUI ui;
    CardOracle* mycards;
public:
    FrontEnd(): io(), ui(&io) { io.setMenuScene(); }
    std::string getName();
    std::string getLogin();
    std::ifstream getDeck();

    void create(const char* descr);
    void update(const char* descr);
    void del(const char* descr);
    void bulkOp(const char* descr);
    bool askMulligan();
};

#endif //OLYMPUS_FRONTEND_H
