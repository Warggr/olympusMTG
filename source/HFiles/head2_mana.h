#ifndef OLYMPUS_HEADER_2_MANA_H
#define OLYMPUS_HEADER_2_MANA_H

std::string m2t(Mana m);
Mana t2m(const char* x);
bool less(Mana cost, Mana pool);
Mana minus(Mana pool, Mana cost);
char mana2color(Mana m);

void disp_mana(Mana m, int x, int y);

#endif //OLYMPUS_HEADER_MANA_7_H)