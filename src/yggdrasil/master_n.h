#ifndef OLYMPUS_MASTER_N_H
#define OLYMPUS_MASTER_N_H

#include "abstract_n.h"
#include <vector>

class BoardN;

class MasterN: public AbstractN {
    std::vector<BoardN*> boards;
public:
    void addPlayerBoard(BoardN* board) { boards.push_back(board); }
};

#endif //OLYMPUS_MASTER_N_H
