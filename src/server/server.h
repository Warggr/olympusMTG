#ifndef OLYMPUS_SERVER_H
#define OLYMPUS_SERVER_H

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
	void addPlayerX(const std::list<playerType>& types);
	void launchGame();
};

#endif //OLYMPUS_SERVER_H
