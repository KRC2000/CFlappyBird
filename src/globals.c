#include "globals.h"
#include <stdbool.h>

static Globals globals;

void setGlobals() {
	globals.drawColliders = true;
	globals.state = PLAY;
	globals.time = 0;
	globals.screenWidth = 450;
	globals.screenHeight = 800;
	globals.speed = 100;
	globals.gravity = 800;
}

Globals* getGlobals() {
    return &globals;
}
