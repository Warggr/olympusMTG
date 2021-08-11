#ifndef OLYMPUS_YGGDRASIL_PART_3
#define OLYMPUS_YGGDRASIL_PART_3

#include "headB_board.h"
#include "collection_tn.h"

/*template<typename T>
bool CollectionTN<T>::try_to_construct(Card* to_add, Identifier casted_id, Player* pl) {
    if(!fulfills(casted_id, min_chars, min_requ)) return false;
    for(auto iter = children.begin(); iter != children.end(); ++iter){
        if((*iter)->try_to_construct(to_add, casted_id, pl)) return true;
    }
    construct_pure_child(to_add, pl); //TODO: merge it with a similar child
    return true;
}*/

#endif //OLYMPUS_YGGDRASIL_PART_3