#include "networkwriter.h"
#include "oracles/filereader/binary/binarywriter.h"
#include "Mana/cost.h"

void BinaryBufferWriter::write(const char *chars, long size) {
    if(ptr + size >= Networker::BUFFER_SIZE) {
#ifdef F_STRICT
        if(last_save == 0) throw NetworkError();
#endif
        networker->send(buffer, last_save);
        int i;
        for(i=0; last_save + i < ptr; i++) {
            buffer[i] = buffer[last_save + i];
        }
        ptr = i; last_save = 0;
    }
    for(int i=0; i<size; i++) buffer[ptr + i] = chars[i];
    ptr += size;
}
