#ifndef OLYMPUS_OPTIONWRAPPERS_H
#define OLYMPUS_OPTIONWRAPPERS_H

#include <vector>
#include <memory>

class Option; class Player;

struct OptionWrapper {
    virtual ~OptionWrapper() = default;
    virtual Option* chooseOptionAction() = 0;
};

template<typename T>
struct SpecificOptionWrapper: public OptionWrapper {
    std::vector<T*> options;
};

#endif //OLYMPUS_OPTIONWRAPPERS_H
