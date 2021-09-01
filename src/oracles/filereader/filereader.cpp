#include "filereader.h"

#define OLYMPUS_BINARYCOMPAT_VERSION 1

void PlainFileReader::check_safepoint(char c, const char* message){
	char x = ifile.get();
	if(x != c){
		raise_error(std::string(message) + ": expected '" + c + "', got '" + x + "' instead");
	}
}

void PlainFileReader::raise_error(const std::string& message){
	gdebug(DBG_READFILE | DBG_IMPORTANT) << "Error: at position " << ifile.tellg() << ":" << message << "\n";
	throw DeckbuildingError(message);
}

void PlainFileReader::readName(std::string& name) {
    char cardname[30];
    check_safepoint('"', "before card name"); //<
    ifile.get(cardname, 30, '"'); //get max. 30 characters, ending with '>'
    check_safepoint('"', "after card name");
    check_safepoint(' ', "after card name"); //space
    name = std::string(cardname);
    gdebug(DBG_READFILE) << "Read name: '" << name << "'\n";
}

void PlainFileReader::readManaCost(Mana& mana) {
    char tmp[20];
    ifile >> tmp; //should end with ' '
    mana = Mana(tmp);
    pedantic_safepoint(' ', "after mana cost");
    gdebug(DBG_READFILE) << "Read cost: '" << mana.m2t() << "'\n";
}
