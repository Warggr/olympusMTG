#include "gameplay/2cards.h"
#include "filereader.h"
#include "binary/binaryreader.h"
#include "plain/filereader.h"
#include "plain/Dictionary/dictholder.h"
#include "leveldb/db.h"
#include <sstream>

Deck parseDeck(const std::vector<OracleDescr>& descrs) {
    static DictHolder* dictHolder = nullptr;
    static leveldb::DB* ondisk_cards = nullptr;
    leveldb::Options options;
    options.create_if_missing = true;
    Deck ret;
    ret.oracles = std::vector<CardOracle>(descrs.size());

    for (uint i = 0; i<descrs.size(); i++) {
        std::string str = descrs[i].initializer;
        switch (descrs[i].type) {
            case customcard: {
                std::istringstream stream(str, std::ios::in);
                if (dictHolder == nullptr) dictHolder = new DictHolder();
                PlainFileReader reader(dictHolder, stream);
                ret.oracles[i].init(reader);
            }
            break;
            case reference: {
                if (ondisk_cards == nullptr) {
                    leveldb::Status status = leveldb::DB::Open(options, "database/carddb", &ondisk_cards);
                    std::cout << "Status: " << status.ToString() << "\n";
                    assert(status.ok());
                }
                char buffer[128];
                int j;
                for(j=1; descrs[i].initializer[j] != '>'; ++j) buffer[j-1] = descrs[i].initializer[j];
                buffer[j-1] = 0;
                leveldb::Status status = ondisk_cards->Get(leveldb::ReadOptions(), std::string(buffer), &str);
                if (!status.ok()) {
                    throw DeckbuildingError("No card named '" + std::string(buffer) + "' in the database");
                }
            } [[fallthrough]];
            case compiled_customcard: {
                std::istringstream stream(str, std::ios::in);
                BinaryReader reader(stream);
                ret.oracles[i].init(reader);
            }
        }
        for (int j = 0; j < descrs[i].nb; j++) {
            ret.cards.emplace_back(&(ret.oracles[i]));
        }
    }
    ret.cards.shrink_to_fit();
    delete ondisk_cards;
    return ret;
}
