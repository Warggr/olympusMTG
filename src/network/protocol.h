#ifndef OLYMPUS_PROTOCOL_H
#define OLYMPUS_PROTOCOL_H

#include <string_view>

extern std::string_view id_client, id_server, version_server, version_client;

enum operations : char { CREATE, UPDATE, DELETE, MESSAGE, COMPILED_DECK, KEEPS_HAND, CHOOSE_AMONG, GET_OPTION };

enum entities : char { CARDWRAPPER };

#endif //OLYMPUS_PROTOCOL_H
