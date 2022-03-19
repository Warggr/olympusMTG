#ifndef OLYMPUS_FILEREADER_H
#define OLYMPUS_FILEREADER_H

#include "gameplay/3player_cards.h"

enum oracle_type { reference, customcard, compiled_customcard };

struct OracleDescr {
    oracle_type type;
    unsigned char nb;
    std::string initializer;
    OracleDescr(oracle_type type, unsigned char nb, std::string initializer):
            type(type), nb(nb), initializer(std::move(initializer)) {};
    OracleDescr(oracle_type type, unsigned char nb, const char* initializer):
            type(type), nb(nb), initializer(initializer) {};

    friend std::ostream& operator<<( std::ostream& output, const OracleDescr& desc );
};

Deck parseDeck(const std::vector<OracleDescr>& deck);

#endif //OLYMPUS_FILEREADER_H
