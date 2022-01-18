#include "implementIO.h"
#include "6abstractIO.h"

AbstractIO* AbstractIO::factory() {
    return new ImplementIO();
}
