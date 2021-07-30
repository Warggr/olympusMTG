#include ".header_link.h"
#include "headI_identifiers.h" //auto-general by 4permanents.h

Identifier CardOracle::generate_casted_id() const {
    if(1 <= type && type <= 4){
    	return construct_id_perm(static_cast<permanent_type>(type - 1), color, 0); //TODO add controller
    }
    else return 0; //TODO: implement
}

Identifier Creature::reload_id() const {
	Identifier ret = construct_id_perm(permanent_type::creature, color, 0);
    return ret;
}

Identifier Land::reload_id() const {
	Identifier ret = construct_id_perm(permanent_type::land, color, 0);
    return ret;
}

Identifier Artifact::reload_id() const {
	Identifier ret = construct_id_perm(permanent_type::artifact, color, 0);
    return ret;
}

Identifier Planeswalker::reload_id() const {
	Identifier ret = construct_id_perm(permanent_type::planeswalker, color, 0);
    return ret;
}

Identifier Card::reload_id() const {
    return 0; //TODO
}

Identifier Player::reload_id() const {
    return construct_id_player(player_id);
}

Identifier Spell::reload_id() const {
    return construct_id_spell(true, source);
}

Identifier Resolvable::reload_id() const {
    return construct_id_spell(false, 0);
}

Identifier GameObject::identify(){
	if(!id_still_correct){
    	identifier = reload_id();
    	id_still_correct = true;
    }
    return identifier;
}