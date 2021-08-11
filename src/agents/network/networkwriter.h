#ifndef OLYMPUS_NETWORKWRITER_H
#define OLYMPUS_NETWORKWRITER_H

#include "oracles/filereader/binarywriter.h"
#include "network.h"

class BinaryBufferWriter: public BinaryWriter {
    char buffer[Networker::BUFFER_SIZE]{};
    int last_save, ptr;
    Networker* networker;
    void canary(char canary) override;
    void write(const char *chars, long size) override;
    void savepoint() override { last_save = ptr; }
public:
    explicit BinaryBufferWriter(Networker* networker): networker(networker), last_save(0), ptr(0) {};
};

#endif //OLYMPUS_NETWORKWRITER_H
