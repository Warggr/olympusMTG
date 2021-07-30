#ifndef OLYMPUS_ABSTR_PERMANENT_TN_H
#define OLYMPUS_ABSTR_PERMANENT_TN_H

class APPermanentN{
public:
    APPermanentN* pprev; //this is mainly for convenience, and also probably redundant,
    APPermanentN* pnext;

    APPermanentN(APPermanentN* p, APPermanentN* n): pprev(p), pnext(n) {
        if(pnext) pnext->pprev = this;
    }
    virtual void single_out() = 0; //removes object from its siblings
    virtual void obliterate() = 0; //unconnects from siblings & parents, signal its death to its parents (TODO) and deletes itself
    virtual void unstate() = 0; //changes state
    virtual const Permanent& get_describedObject(const Permanent* signature_compl) const = 0;
    virtual Permanent& get_describedObject(Permanent* signature_compl) = 0;
};

#endif //OLYMPUS_ABSTR_PERMANENT_TN_H
