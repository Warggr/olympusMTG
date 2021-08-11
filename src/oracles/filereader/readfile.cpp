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
    for(auto& desc : deck) {
        card_ptr oracle = nullptr;
        std::string str = desc.initializer;
        switch(desc.type) {
            case customcard: {
                std::istringstream stream(str, std::ios::in);
                if(dictHolder == nullptr) dictHolder = new DictHolder();
                PlainFileReader reader(dictHolder, stream);
                oracle = new_cardptr(reader);
            } break;
            case reference: {
                if(ondisk_cards == nullptr) {
                    leveldb::Status status = leveldb::DB::Open(options, "material/database/carddb", &ondisk_cards);
                    assert(status.ok());
                }
                leveldb::Status status = ondisk_cards->Get(leveldb::ReadOptions(), desc.initializer, &str);
            } [[fallthrough]];
            case compiled_customcard: {
                std::istringstream stream(str, std::ios::in);
                BinaryReader reader(stream);
                oracle = new_cardptr(reader);
            }
        }
        for(int i=0; i<desc.nb; i++) {
            ret.takeonecard(std::make_unique<Card>(oracle));
        }
    }
    return ret;
}
