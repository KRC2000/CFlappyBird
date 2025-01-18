#ifndef GLOBALS_H
#define GLOBALS_H

#include <stddef.h>

typedef struct {
    int screenWidth;
    int screenHeight;
	float speed;
} Globals;

Globals* getGlobals();


#endif
