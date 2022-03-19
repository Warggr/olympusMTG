#ifndef OLYMPUS_PROTOCOL_H
#define OLYMPUS_PROTOCOL_H

extern const char* id_client, * id_server, * version_server, * version_client;

enum operations : char { CREATE, UPDATE, DELETE, MESSAGE, COMPILED_DECK, KEEPS_HAND };

#endif //OLYMPUS_PROTOCOL_H
