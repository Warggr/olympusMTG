#ifndef OLYMPUS_CARDINHAND_H
#define OLYMPUS_CARDINHAND_H

class CardInHand : public SpecificOptionWrapper<SpellOption> {
    uptr<Card> origin;

    std::unique_ptr<OptionAction> chooseOptionAction() override;
};

#endif //OLYMPUS_CARDINHAND_H
