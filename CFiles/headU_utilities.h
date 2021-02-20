#ifndef OLYMPUS_HEAD1_UTILITIES_H
#define OLYMPUS_HEAD1_UTILITIES_H

void set_canary(char canary, std::ofstream& bFile);
void check_canary(char canary, std::ifstream& bFile);

#endif //OLYMPUS_HEAD1_UTILITIES_H
