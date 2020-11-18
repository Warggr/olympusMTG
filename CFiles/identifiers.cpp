#include "../HFiles/classes.h"

/* An identifier will be an int containing all relevant properties of a game object, which can be a:
player (000) - permanent(001) - creature(010) - zone(011) - ability(100) - card(101) - spell(110) - enums(111)
it is more generally usable than a pointer
FIRST 3 BITS: type of object
SECOND  BYTE: subtype of type
THIRD BYTE: other object informations
*/