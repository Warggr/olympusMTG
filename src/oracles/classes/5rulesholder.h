#ifndef OLYMPUS_RULESHOLDER_H
#define OLYMPUS_RULESHOLDER_H

class Effect_H; class ActAb_H; class Trigger_H; class StaticAb_H;
class ReaderVisitor; class TriggerHolder_H;

struct RulesHolder {
    Effect_H* on_cast;
    uint nb_actabs;
    ActAb_H* first_actab;
    uint nb_triggers;
    TriggerHolder_H* triggers;
    uint nb_statics;
    StaticAb_H* statics;
    char* flavor_text; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
    //Static abilities go into that category until we implement them.
    //In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
    //BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA

    inline void get_read(ReaderVisitor& visitor, card_type type);
};

#endif //OLYMPUS_RULESHOLDER_H
