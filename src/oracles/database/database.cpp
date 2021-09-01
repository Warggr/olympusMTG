#include "server/networkagent.h"
#include "oracles/filereader/filereader.h"
#include "oracles/filereader/binarywriter.h"
#include "build_types.h"
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
        gdebug(DBG_IMPORTANT | DBG_READFILE) << "Error: no oracles.txt file\n";
        exit(1);
    }

    DictHolder* dicts = new DictHolder();
    while(fb.peek() != EOF) {
        char buffer[1024];
        fb.getline(buffer, 1024);

        std::istringstream ss {std::string(buffer)};
        PlainFileReader reader(dicts, ss);

        std::string binary;
        std::ostringstream oss(binary);
        BinaryFileWriter writer(oss);

        auto* oracle = new CardOracle(reader);
        oracle->get_read(writer);

        status = db->Put(leveldb::WriteOptions(), oracle->getName(), binary);
        assert(status.ok());
        delete oracle;
    }

    delete dicts;
    delete db;
}
