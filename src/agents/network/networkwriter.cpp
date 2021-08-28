#include "networkwriter.h"
#include "oracles/filereader/binarywriter.h"

void BinaryBufferWriter::write(const char *chars, long size) {
    if(ptr + size >= Networker::BUFFER_SIZE) {
#ifdef F_STRICT
        if(last_save == 0) throw NetworkError();
#endif
        networker->net_send(buffer, last_save);
        int i;
        for(i=0; last_save + i < ptr; i++) {
            buffer[i] = buffer[last_save + i];
        }
        ptr = i; last_save = 0;
    }
    for(int i=0; i<size; i++) buffer[ptr + i] = chars[i];
    ptr += size;
}

void BinaryBufferWriter::canary(char canary) {
    (void) canary; //As of now, does nothing, because we want to keep the data size as small as possible
}
