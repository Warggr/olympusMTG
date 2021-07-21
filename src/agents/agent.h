#ifndef OLYMPUS_AGENT_H
#define OLYMPUS_AGENT_H

#include <iostream>
#include <memory>

class Option;

enum playerType { /*LOCAL, BOT,*/ NETWORK  };

class Agent {
public:
    virtual ~Agent() = default;
    virtual void setup() = 0;
    virtual std::string getName() = 0;
	//virtual Option* chooseOpt() = 0;
	//virtual std::ifstream getDeck() = 0;
};

std::unique_ptr<Agent> createAgent(playerType desc);

#endif //OLYMPUS_AGENT_H
