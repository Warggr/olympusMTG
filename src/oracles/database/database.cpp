#include "oracles/filereader/plain/filereader.h"
#include "oracles/filereader/binary/binarywriter.h"
#include "oracles/filereader/plain/Dictionary/dictholder.h"
#include "oracles/filereader/visit.hpp"
#include "classes/card_oracle.h"
#include "logging.h"
#include "leveldb/db.h"
#include <fstream>
#include <sstream>

void refreshDatabase() {
    leveldb::DB* db;
    leveldb::Options options;
    options.create_if_missing = true;

    leveldb::Status status = leveldb::DB::Open(options, "database/carddb", &db);
    assert(status.ok());

    std::ifstream fb("database/oracles.txt", std::ios::in);
    if(!fb) {
        gdebug(DBG_READFILE) << "Error: no oracles.txt file\n";
        exit(1);
    }

    DictHolder dicts;
    while(fb.peek() != EOF) {
        char buffer[1024];
        fb.getline(buffer, 1024);

        std::istringstream ss {std::string(buffer)};
        PlainFileReader reader(&dicts, ss);

        std::string binary;
        std::ostringstream oss(binary);
        BinaryWriter writer(oss);

        CardOracle oracle;
        visit<true>(oracle, reader);
        visit<false>(oracle, writer);

        status = db->Put(leveldb::WriteOptions(), oracle.getName(), binary);
        assert(status.ok());
    }

    delete db;
}
