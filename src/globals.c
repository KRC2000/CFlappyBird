#include "globals.h"

static Globals globals = { 0, 450, 800, 100, 800 };

Globals* getGlobals() {
    return &globals;
}
