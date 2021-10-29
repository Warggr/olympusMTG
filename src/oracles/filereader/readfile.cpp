#include "control/7game.h"
#include "binaryreader.h"
#include "filereader.h"
#include "leveldb/db.h"
#include <sstream>

CardZone parseDeck(const std::vector<OracleDescr>& deck) {
    static DictHolder* dictHolder = nullptr;
    static leveldb::DB* ondisk_cards = nullptr;
    leveldb::Options options;
    options.create_if_missing = true;
    CardZone ret;
    for (auto& desc : deck) {
        card_ptr oracle = nullptr;
        std::string str = desc.initializer;
        switch (desc.type) {
            case customcard: {
                std::istringstream stream(str, std::ios::in);
                if (dictHolder == nullptr) dictHolder = new DictHolder();
                PlainFileReader reader(dictHolder, stream);
                oracle = new_cardptr(reader);
            }
                break;
            case reference: {
                if (ondisk_cards == nullptr) {
                    leveldb::Status status = leveldb::DB::Open(options, "database/carddb", &ondisk_cards);
                    std::cout << "Status: " << status.ToString() << "\n";
                    assert(status.ok());
                }
                char buffer[128];
                int i;
                for(i=1; desc.initializer[i] != '>'; ++i) buffer[i-1] = desc.initializer[i];
                buffer[i-1] = 0;
                leveldb::Status status = ondisk_cards->Get(leveldb::ReadOptions(), std::string(buffer), &str);
                if (!status.ok()) {
                    throw DeckbuildingError("No card called '" + std::string(buffer) + "' in the database");
                }
            } [[fallthrough]];
            case compiled_customcard: {
                std::istringstream stream(str, std::ios::in);
                BinaryReader reader(stream);
                oracle = new_cardptr(reader);
            }
        }
        for (int i = 0; i < desc.nb; i++) {
            ret.takeonecard(std::make_unique<Card>(oracle));
        }
    }
    delete ondisk_cards;
    return ret;
}
