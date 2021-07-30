#ifndef OLYMPUS_LOCALAGENT_H
#define OLYMPUS_LOCALAGENT_H

#include "../agent.h"
#include "frontend/frontend.h"

class LocalAgent: public Agent {
    FrontEnd frontEnd;
public:
    LocalAgent() = default;
    void specificSetup() override {};
    std::string getName() override { return frontEnd.getName(); }
    //Option* chooseOpt() override;
    std::vector<OracleDescr> getDeck() override;
    Target* chooseTarget(char type) override;
};

#endif //OLYMPUS_LOCALAGENT_H
