#ifndef OLYMPUS_NETWORKAGENT_H
#define OLYMPUS_NETWORKAGENT_H

#include "agents/agent.h"
#include <list>
#include <memory>
#include <forward_list>
class Game;

class Server {
	std::unique_ptr<Game> currentGame;
	std::list<std::unique_ptr<Agent>> players;
	//std::list<Watcher> spectators;
public:
    static Server* god;
	Server();
	~Server();

	void addPlayer(playerType type);
	void addPlayers(const std::list<playerType>& types);
	void launchGame();
};

void refreshDatabase();

#endif //OLYMPUS_NETWORKAGENT_H
