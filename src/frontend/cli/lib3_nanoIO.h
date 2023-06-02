#ifndef OLYMPUS_11_NANOIO
#define OLYMPUS_11_NANOIO

#include <iostream>
#include "frontend/basicIO.h"

class Permanent; class Card; class Creature; class CardOracle; class Option; class Player; class Resolvable;

class NanoIO: public BasicIO, public Canvas {
public:
    NanoIO();
    ~NanoIO();

    void setMenuScene();
    void setGameScene();

    void message(std::string_view) const override;

    int getInt(int lowerBound, int upperBound) override;
    std::string getTextInput(const char* question, bool newline);
    inline std::string getTextInput(const char* question) override { return getTextInput(question, true); }
    bool simpleChoice(const char* optTrue, const char* optFalse) override;

    void draw_permanent(const std::string& name, char color, bool tapped, bool highlight) const;
    void disp_player(const Gamer& player, int flags) const override;
    void disp_cardback(int oncard_number) const;
    void disp(const CardOracle& oracle, int flags) const override;
    void disp_inrow(const Displayable* disp, int number, int total, int flags) const override;
    void disp(const Card& card, int flags = 0) const;
    void disp(const Option& option, int flags = 0) const;

    void poster(const CardOracle& card) const { disp(card, HIGHLIGHT); }

    template<class Container> uint chooseAmong(Container container) {
        if(container.size() == 1) return 0;
        message("Choose among these:\n");
        int i = 0;
        for(auto obj = container.begin(); obj != container.end(); ++obj, ++i) {
            std::cout << i << "\t";
            disp(*obj, INLINE);
        }
        return getInt(0, container.size());
    }

    checklistCallbackAction getNextPosition(abstract_iterator_wrapper* iter, uint &position, uint max) override;
};

#endif //OLYMPUS_11_NANOIO
