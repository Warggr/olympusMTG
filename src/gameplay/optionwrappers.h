#ifndef OLYMPUS_OPTIONWRAPPERS_H
#define OLYMPUS_OPTIONWRAPPERS_H

#include <vector>
#include <memory>

class Option;

/**
 * An OptionWrapper is an object that contains multiple options that are somehow related to each other.
 * When choosing an Option, the user will usually select an OptionWrapper and then choose one of the contained Options.
 * Grouping Options into OptionWrapper is also useful because they often need to be deleted / invalidated at the same time.
 */
struct OptionWrapper {
    virtual ~OptionWrapper() = default;
    virtual const Option* chooseOptionAction() const = 0;
};

template<typename T>
struct SpecificOptionWrapper: public OptionWrapper {
    std::vector<T*> options;
};

#endif //OLYMPUS_OPTIONWRAPPERS_H
