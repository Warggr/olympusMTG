#include "7game.h"

int main() {
    std::list<uptr<Agent>> agents;
    for(int i=0; i<2; i++) agents.push_front(nullptr);
    Game game(agents);
    game.play();
}
