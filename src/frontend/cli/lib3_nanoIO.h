#ifndef OLYMPUS_11_NANOIO
#define OLYMPUS_11_NANOIO

#include <iostream>
#include "frontend/basicIO.h"

class Permanent; class Card; class Creature; class CardOracle; class EmptyOption; class Player; class Resolvable;

class NanoIO: public BasicIO {
public:
    static constexpr int INLINE = 1, HIGHLIGHT = 2;

    NanoIO();
    ~NanoIO();

    void setMenuScene();
    void setGameScene();

    void message(const char* message) const override;
    void message(const std::string& text) const override;

    int getInt(int lowerBound, int upperBound) override;
    std::string getTextInput(const char* question) override;
    bool simpleChoice(const char* optTrue, const char* optFalse) override;

    template<typename T> void disp(T* obj, int flags = 0) const { disp(*obj, flags); }
    void draw_permanent(const std::string& name, char color, bool tapped, bool highlight) const;
    void disp(const Player& player, int flags = 0) const;
    void disp_cardback(int oncard_number) const;
    void disp(const CardOracle& oracle, int flags = 0) const;
    void disp(const uptr<Card>& card, int flags = 0) const;
    void disp(const Option& option, int flags = 0) const;

    void poster(const CardOracle& card) const { disp(card, HIGHLIGHT); }

    bool attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const;

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

    template<typename T>
    std::list<T> checklist(std::list<T>& list, uint min, uint max) {
        std::list<T> ret;
        uint topMargin = max, lowMargin = list.size() - min;
        int i = 0;
        for(auto obj = list.begin(); obj != list.end(); ++i) {
            if(topMargin == 0) return ret;
            if(lowMargin == 0) {
                ret.splice(ret.end(), list, obj, list.end());
                return ret;
            }
            std::cout << i << "\t";
            disp(*obj, INLINE);
            auto cp = obj;
            obj++;
            bool keep = simpleChoice("Select", nullptr);
            if(keep) {
                ret.splice(ret.end(), list, cp);
                --topMargin;
            } else {
                --lowMargin;
            }
        }
        return ret;
    }
};

#endif //OLYMPUS_11_NANOIO
