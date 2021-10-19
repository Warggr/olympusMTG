#ifndef OLYMPUS_CARDINHAND_H
#define OLYMPUS_CARDINHAND_H

class CardInHand : public SpecificOptionWrapper<CardOption> {
    uptr<Card> origin;

    Option* chooseOptionAction() override;
};

#endif //OLYMPUS_CARDINHAND_H
