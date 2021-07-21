#ifndef OLYMPUS_HEAD_I_IDENTIFIERS
#define OLYMPUS_HEAD_I_IDENTIFIERS

enum object_type { player, permanent, resolvable, card };
enum permanent_type { land, artifact, planeswalker, creature };
enum card_type { landc, artifactc, planeswalkerc, creaturec, instantc, sorceryc};
enum mtgcolor { white, blue, black, red, green };

constexpr Identifier rid_objtype = 0x3;
constexpr Identifier rid_perm_type = 0x1C;//0b11100;
constexpr Identifier rid_colors_exact = 0x3E0;//0b1111100000;
constexpr Identifier rid_controller = 0x400;
constexpr Identifier rid_tribe = 0x3800;

constexpr inline Identifier cid_objtype(int type){
	Identifier ret = type; return ret;
}
constexpr inline Identifier cid_perm_type(int type){
    Identifier ret = type*0b10; return ret*0x4;
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
    return cid_objtype(permanent) + cid_perm_type(type) + cid_color(color) + cid_controller(controller ? 1 : 0);
}
constexpr inline Identifier construct_id_player(char player_id){
    return ((Identifier) player) + player_id*0b100;
}
inline Identifier construct_id_spell(bool is_spell, Card* origin){
    Identifier ret = ((Identifier) resolvable) + (is_spell ? 0x20 : 0);
    return ret; //TODO add info about the card, e.g. CMC
}
//enums seem too complex, we'll do them later
/* An identifier will be an int containing all relevant properties of a game object, which can be a:
player(00) - permanent(01) - resolvable(10) - card(11)
 types: land 000 - artifact 010 - planeswalker 100 - creature 110 - sorcery 001 - instant 011
 (damageable: 0X01X)
 (zone, enum, spell and ability/spell also envisaged)
Some special Identifiers might include 'this' and 'you' (aka 'this object's controller') (TODO)
Controllers: you (0), an opponent (1)*/

#endif //OLYMPUS_HEAD_I_IDENTIFIERS