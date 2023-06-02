#ifndef OLYMPUS_SERVER_H
#define OLYMPUS_SERVER_H

#include "agents/agent.h"
#include "agents/agent_types.h"
#include "network/server/networkmanager.h"
#include <list>
#include <memory>

class Game;

class Server {
	std::unique_ptr<Game> currentGame;
	std::list<std::unique_ptr<Agent>> players;
	std::list<Viewer*> spectators;
    ENABLE_IF_NETWORK( NetworkManager manager; )
public:
	Server();
	~Server();

	void addPlayer(PlayerType type);
	void addPlayers(const std::list<PlayerType>& types);
	void launchGame();

	const std::list<std::unique_ptr<Agent>>& getAgents() { return players; }

    void addToLog(std::string_view message);
};

void refreshDatabase();

#endif //OLYMPUS_SERVER_H
