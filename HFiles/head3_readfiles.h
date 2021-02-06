#ifndef OLYMPUS_HEAD_3_READFILES
#define OLYMPUS_HEAD_3_READFILES

#include <iostream>

#define MANUAL_IMPORT_OF_OLYMPUS_HEADERS
#include "../include/.olympus_main.h"
#undef MANUAL_IMPORT_OF_OLYMPUS_HEADERS

bool read_section_flavor(std::ifstream& myfile, char*& flavor_text, char offset_text);
bool read_section_abils(std::ifstream& myfile, int& nb_actabs, PermOption*& first_actab);
bool read_section_triggers(std::ifstream& myfile, int& nb_triggers, char*& typeof_triggers, Trigger*& triggers);
bool read_section_onresolve(std::ifstream& myfile, PreResolvable*& preRes);
bool read_section_statics(std::ifstream& myfile, int& nb_statics, StaticAb*& statics);

void check_safepoint(std::ifstream& myfile, char c, const char* message);
void check_safepoint(std::ifstream& myfile, char c, const char* message);

void set_canary(char canary, std::ofstream& bFile);
void check_canary(char canary, std::ifstream& bFile);

int nb_phrases(std::ifstream& myfile, char& v);

char read_typeof_target(const char* txt);
//player 80 - permanent 40 - spell (well, resolvable) 20 - creature 10 - planeswalker 08 - ? - card 02 - ?

int read_ability_type(std::ifstream& myfile);
char read_ability_parameter(std::ifstream& myfile, char* allassignedvariables, int* nbassignedparams);
void read_staticeffect(std::ifstream& myfile, char& first_effect, char*& other_effects, char& nbeffects);
void read_selector(std::ifstream& myfile, Identifier& chars, Identifier& requs);
bool read_one_criterion(std::ifstream& myfile, Identifier& chars, Identifier& requs);

#endif //OLYMPUS_HEADER_9_READFILES