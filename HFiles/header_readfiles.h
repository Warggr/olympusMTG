#ifndef OLYMPUS_HEADER_9_READFILES
#define OLYMPUS_HEADER_9_READFILES

void check_safepoint(FILE* myfile, char c, const char* message);
void check_safepoint(FILE* myfile, char c, const char* message);

void read_rules_text(FILE* myfile, Ability** fab, char** flavor_text, char** parameter_list, int* nb_parameters, int* nb_actabs, PermOption** first_actab, char type);
bool read_section_onresolve(FILE* myfile, Ability** ab, int* nbargs, char** args);
bool read_section_abils(FILE* myfile, int* nb_actabs, PermOption** first_actab);
bool read_section_flavor(FILE* myfile, char** flavor_text, char offset_text);

char read_typeof_target(FILE* myfile);

int read_ability_type(FILE* myfile);
char read_ability_parameter(FILE* myfile, char* allassignedvariables, int* nbassignedparams, bool secondtime);

#endif //OLYMPUS_HEADER_9_READFILES