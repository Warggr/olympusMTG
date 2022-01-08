#ifndef OLYMPUS_NETWORKWRITER_H
#define OLYMPUS_NETWORKWRITER_H

#include <ostream>
#include "network.h"

class BinaryBufferWriter: public std::ostream {
    char buffer[Networker::BUFFER_SIZE]{};
    int last_save, ptr;
    Networker* networker;
    void write(const char *chars, long size);
    void savepoint() { last_save = ptr; }
public:
    explicit BinaryBufferWriter(Networker* networker): last_save(0), ptr(0), networker(networker) {};
};

#endif //OLYMPUS_NETWORKWRITER_H
