#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdbool.h>
#include "raylib.h"

enum GameState { PLAY, MENU, DEATH };

typedef struct {
	bool drawColliders;
	enum GameState state;
	float scale;
	float time;
	int screenWidth;
	int screenHeight;
	unsigned int score;
	unsigned int bestScore;
	float speed;
	float gravity;
	int bottomPadding;
	Vector2 birdStartPos;
} Globals;

void setGlobals();
Globals* getGlobals();
void writeBestScore(unsigned int s);

#endif
