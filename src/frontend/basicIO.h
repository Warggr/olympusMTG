#ifndef OLYMPUS_BASICIO_H
#define OLYMPUS_BASICIO_H

#include "displayable.h"
#include "boost/format.hpp"
#include <list>
#include <vector>
#include <iterator>
#include <string>

class CardOracle; class Target; class Card; class Gamer; class CardWrapper; class Creature;
template<typename T> class StateTN;
class PermOption;

struct Canvas {
    virtual ~Canvas() = default;
    virtual void disp(const CardOracle& oracle, int flags) const = 0;
    virtual void disp_player(const Gamer& player, int flags) const = 0;
};

class BasicIO {
protected:
    enum checklistCallbackAction { commit, change };
    class abstract_iterator_wrapper {
    public:
        virtual ~abstract_iterator_wrapper() = default;
        virtual abstract_iterator_wrapper& operator++() = 0;
        virtual abstract_iterator_wrapper& operator--() = 0;
    };
    template<typename iter> class iterator_wrapper: public abstract_iterator_wrapper {
    public:
        iter inner;
        constexpr iterator_wrapper(iter i): inner(i) {};
        abstract_iterator_wrapper& operator++() override { ++inner; return *this; }
        abstract_iterator_wrapper& operator--() override { ++inner; return *this; }
    };

    template<typename T> const Displayable* to_disp(const T* t) { return t; }
    template<typename T> const Displayable* to_disp(const uptr<T>& t) { return t.get(); }
    inline const CardWrapper* to_disp(const CardWrapper& card) { return &card; }
    inline const PermOption* to_disp(const PermOption& opt) { return &opt; }
    virtual checklistCallbackAction getNextPosition(abstract_iterator_wrapper* iter, uint& position, uint max) = 0;
public:
    static constexpr int INLINE = 1, INROW = 2, HIGHLIGHT = 4, SELECTED = 8;
    virtual ~BasicIO() = default;

    virtual void message(std::string_view message) const = 0;
    virtual void disp_inrow(const Displayable*, int number, int total, int flags) const = 0;

    virtual std::string getTextInput(const char* question) = 0;
    virtual int getInt(int lowerBound, int upperBound) = 0;
    virtual bool simpleChoice(const char* optTrue, const char* optFalse) = 0;

    void chooseAttackers(StateTN<Creature>& attackers);

    template<typename O>
    uint chooseAmong(const std::vector<O> all) {
        iterator_wrapper<typename std::vector<O>::const_iterator> wrapper = all.begin();
        uint pos = 0;
        int i = 0;
        for(auto iter = all.begin(); iter != all.end(); ++iter, ++i)
            disp_inrow(to_disp(*iter), i, all.size(), 0);
        while(true) {
            auto action = getNextPosition(&wrapper, pos, all.size());
            if(action == commit) return pos;
        }
    }

    template<typename O>
    std::list<O> checklist(std::list<O>& all, uint min, uint max) {
        std::list<O> ret;
        if(max == 0) return ret;
        if(min == all.size()) {
            ret.splice(ret.begin(), all);
            return ret;
        }
        uint nbSelected = 0;
        std::vector<bool> selected(all.size() + 1, false);

        iterator_wrapper<typename std::list<O>::iterator> wrapper = all.begin();
        uint pos = 0;
        while(true) {
            int i=0;
            for(auto iter = all.begin(); iter != all.end(); ++iter, ++i)
                disp_inrow(to_disp(*iter), i, all.size(), selected[i+1] ? SELECTED : 0);
            auto action = getNextPosition(&wrapper, pos, all.size());
            if(action == change) {
                if(selected[pos+1]) nbSelected -= 1;
                else nbSelected += 1;
                selected[pos+1] = !selected[pos+1];
            }
            else if(action == commit and min <= nbSelected and nbSelected <= max) {
                break;
            } else message(
                    (boost::format("Please select between %d and %d (currently: %d)\n")
                    % min % max % nbSelected).str()
                    );
        }
        auto lastPos = all.end(), iter2 = all.begin();
        for(uint i=0; i<all.size(); ++i, ++iter2) if(selected[i] != selected[i+1]) {
            if(lastPos == all.end()) {
                lastPos = iter2;
            } else {
                ret.splice(ret.end(), all, lastPos, iter2);
                lastPos = all.end();
            }
        }
        if(lastPos != all.end()) ret.splice(ret.end(), all, lastPos, iter2);
        return ret;
    }
};

class UIClosedException: public std::exception{};

#endif //OLYMPUS_BASICIO_H
