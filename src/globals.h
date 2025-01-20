#ifndef GLOBALS_H
#define GLOBALS_H

#include <stddef.h>

typedef struct {
	float time;
    int screenWidth;
    int screenHeight;
	float speed;
	float gravity;
} Globals;

Globals* getGlobals();


#endif
