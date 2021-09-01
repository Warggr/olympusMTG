#ifndef OLYMPUS_YGGDRASIL_1_H
#define OLYMPUS_YGGDRASIL_1_H

#include "oracles/classes/3statics.h"
#include "headB_board.h"
#include "headI_identifiers.h"
#include <list>

class AbstractN{ //TODO: give triggers (different number of triggers depending on the type)
protected:
    ModifListNode* all_modifs;
    Identifier min_chars;
    Identifier min_requ;
public:
    AbstractN() { all_modifs = nullptr; }
    virtual ~AbstractN() = default;
    ModifListNode* add_modif(StaticAb_H* staticAb){ all_modifs = new ModifListNode(staticAb, all_modifs); return all_modifs; };
    void remove_modif(ModifListNode* ptr) {
        if(ptr->prev) ptr->prev->next = ptr->next;
        else all_modifs = ptr->next;
        if(ptr->next) ptr->next->prev = ptr->prev;
    }

    virtual bool empty() const = 0;
};

#endif //OLYMPUS_YGGDRASIL_1_H
