#ifndef OLYMPUS_YGGDRASIL_1_H
#define OLYMPUS_YGGDRASIL_1_H

#include <list>
class StaticAb;

class AbstractN{ //TODO: give triggers (different number of triggers depending on the type)
protected:
    struct ListNode{
        StaticAb* modif;
        ListNode* prev;
        ListNode* next;
        ListNode(StaticAb* st, ListNode* nxt): modif(st), prev(0), next(nxt) {};
    } *all_modifs;
    Identifier min_chars;
    Identifier min_requ;
public:
    AbstractN() { all_modifs = nullptr; }
    virtual ~AbstractN() = default;
    ListNode* add_modif(StaticAb* staticAb){ all_modifs = new ListNode(staticAb, all_modifs); return all_modifs; };
    void remove_modif(ListNode* ptr) {
        if(ptr->prev) ptr->prev->next = ptr->next;
        else all_modifs = ptr->next;
        if(ptr->next) ptr->next->prev = ptr->prev;
    }
};

#endif //OLYMPUS_YGGDRASIL_1_H
