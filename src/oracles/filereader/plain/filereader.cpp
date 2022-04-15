#include "filereader.h"
#include "Mana/cost.h"

void PlainFileReader::checkSafepoint(char c, const char* message){
    char x = ifile.get();
    if(x != c){
        raiseError(std::string(message) + ": expected '" + c + "', got '" + x + "' instead");
    }
}

void PlainFileReader::raiseError(const std::string& message){
    gdebug(DBG_READFILE) << "Error: at position " << ifile.tellg() << ":" << message << "\n";
    throw DeckbuildingError(message);
}

void PlainFileReader::visit(const char*, std::string& name) {
    char cardname[30];
    checkSafepoint('"', "before card name"); //<
    ifile.get(cardname, 30, '"'); //get max. 30 characters, ending with '>'
    checkSafepoint('"', "after card name");
    checkSafepoint(' ', "after card name"); //space
    name = std::string(cardname);
    gdebug(DBG_READFILE) << "Read name: '" << name << "'\n";
}

void PlainFileReader::visit(const char*, Cost& cost) {
    visit("mana", cost.mana);
}

void PlainFileReader::visit(const char*, char&) {
    //TODO ? where is this needed?
}

void PlainFileReader::visit(const char*, Mana& mana) {
    char tmp[20];
    ifile >> tmp; //should end with ' '
    mana = Mana(tmp);
    pedantic_safepoint(' ', "after mana cost");
    gdebug(DBG_READFILE) << "Read cost: '" << mana.m2t() << "'\n";
}

void PlainFileReader::visit(const char*, bool&) {
    //TODO ? where is this needed?
}
