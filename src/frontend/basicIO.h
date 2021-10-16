#ifndef OLYMPUS_BASICIO_H
#define OLYMPUS_BASICIO_H

#include <list>
#include <vector>

class BasicIO {
public:
    virtual ~BasicIO() = default;
    template<typename O> std::list<O> checklist(std::list<O>& all, unsigned min = 0, unsigned max = 0);
    template<typename O> uint chooseAmong(std::vector<O> objects);
    virtual bool simpleChoice(const char* optTrue, const char* optFalse) = 0;

    virtual void message(const char* message) const = 0;
    virtual void message(const std::string& text) const = 0;
    virtual std::string getTextInput(const char* question) = 0;
    virtual int getInt(int lowerBound, int upperBound) = 0;
};

template<typename O>
std::list<O> BasicIO::checklist(std::list<O> &all, unsigned int min, unsigned int max) {
    (void) all; (void) min; (void) max;
    return std::list<O>(); //TODO
}

template<typename O>
uint BasicIO::chooseAmong(std::vector<O> objects) {
    (void) objects;
    return 0; //TODO
}

class UIClosedException: public std::exception{};

#endif //OLYMPUS_BASICIO_H
