#include "lib3_cliUI.h"
#include "lib3_nanoIO.h"


using namespace std;

bool NanoIO::attackSwitch(int, int, int, int) {
	return simpleChoice("Attack", nullptr);
}
