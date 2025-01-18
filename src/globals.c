#include "globals.h"


static Globals globals = { 450, 800, 15 };

Globals* getGlobals() {
    return &globals;
}
