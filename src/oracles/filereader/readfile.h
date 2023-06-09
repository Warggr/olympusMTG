#ifndef OLYMPUS_READFILE_H
#define OLYMPUS_READFILE_H

#include "filereader.h"
#include "gameplay/2cards.h"
#include <vector>

Deck parseDeck(const std::vector<OracleDescr>& descrs);

#endif //OLYMPUS_READFILE_H
