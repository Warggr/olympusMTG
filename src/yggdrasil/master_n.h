#ifndef OLYMPUS_MASTER_N_H
#define OLYMPUS_MASTER_N_H

#include "abstract_n.h"
#include <vector>

class BoardN;

/** The highest class in Yggdrasil. Contains every object on the battlefield. */
class MasterN: public Yggdrasil<Permanent> {
    std::vector<BoardN*> boards;
public:
    void addPlayerBoard(BoardN* board) { boards.push_back(board); }

    iterator<Permanent, false> begin() override;
    iterator<Permanent, false> end() const;

    void disp(unsigned int indent, logging::record_ostream& strm) const override;
};

#endif //OLYMPUS_MASTER_N_H
