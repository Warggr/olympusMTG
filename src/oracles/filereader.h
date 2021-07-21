#ifndef OLYMPUS_FILEREADER_H
#define OLYMPUS_FILEREADER_H

#include <iostream>

class FileReader {
    virtual effect_type readAbilityType() = 0;
    virtual short int readNumber(char a, bool can_be_zero, bool can_be_negative);

    static
};

class PlainFileReader: public FileReader {
    std::istream ifile;
    std::ostream ofile;
public:
    enum punctuation { CONTINUE, ENDPOINT, BREAKOUT };

    short int readNumber(char a, bool can_be_zero, bool can_be_negative);
    punctuation getPunctuation();
    int8_t readAbilityParameter(char* allassignedvariables, uint8_t* nbassignedparams, flag_t type);
};

class BinaryFileReader: public FileReader {
    std::istream ifile;
    short int readNumber(char a, bool can_be_zero, bool can_be_negative);
};

#endif //OLYMPUS_FILEREADER_H
