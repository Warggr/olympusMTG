#include "classes/perm_option.h"
#include "classes/2triggers.h"
#include "classes/3statics.h"
#include "classes/5rulesholder.h"

RulesHolder::~RulesHolder() {
    delete[] first_actab;
    delete[] triggers;
    delete[] statics;
    delete[] flavor_text;
}
