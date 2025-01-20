#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>

enum GameState { PLAY, MENU, DEATH };

typedef struct {
	bool drawColliders;
	enum GameState state;
	float time;
	int screenWidth;
	int screenHeight;
	float speed;
	float gravity;
} Globals;

void setGlobals();
Globals* getGlobals();

#endif
