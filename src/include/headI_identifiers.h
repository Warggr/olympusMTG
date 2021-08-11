#ifndef OLYMPUS_HEAD_I_IDENTIFIERS
#define OLYMPUS_HEAD_I_IDENTIFIERS

#include "headE_enums.h"

using Identifier = int;

class Card;

constexpr Identifier rid_objtype = 0x3;
constexpr Identifier rid_perm_type = 0x1C;//0b11100;
constexpr Identifier rid_colors_exact = 0x3E0;//0b1111100000;
constexpr Identifier rid_controller = 0x400;
constexpr Identifier rid_tribe = 0x3800;

constexpr inline Identifier cid_objtype(int type){
	Identifier ret = type; return ret;
}
constexpr inline Identifier cid_perm_type(permanent_type type){
    Identifier ret = ((int)type)*0b10; return ret*0x4;
}
constexpr inline Identifier cid_tribe(int tribe) {
    Identifier ret = tribe; return ret*0x800;
}
constexpr inline Identifier cid_controller(int controller) {
    return controller*rid_controller;
}
constexpr inline Identifier cid_color(char color) {
    return color*0x20;
}
constexpr inline Identifier construct_id_perm(enum permanent_type type, char color, bool controller){
    return cid_objtype(target_type::permanent) + cid_perm_type(type) + cid_color(color) + cid_controller(controller ? 1 : 0);
}
constexpr inline Identifier construct_id_player(char player_id){
    return ((Identifier) target_type::player) + player_id*0b100;
}
inline Identifier construct_id_spell(bool is_spell, Card* origin){
    Identifier ret = ((Identifier) target_type::resolvable) + (is_spell ? 0x20 : 0);
    return ret; //TODO add info about the card, e.g. CMC
}
//enums seem too complex, we'll do them later
/* An identifier will be an int containing all relevant properties of a game object, which can be a:
local(00) - permanent(01) - resolvable(10) - card(11)
 types: land 000 - artifact 010 - planeswalker 100 - creature 110 - sorcery 001 - instant 011
 (damageable: 0X01X)
 (zone, enum, spell and ability/spell also envisaged)
Some special Identifiers might general 'this' and 'you' (aka 'this object's controller') (TODO)
Controllers: you (0), an opponent (1)*/

#endif //OLYMPUS_HEAD_I_IDENTIFIERS