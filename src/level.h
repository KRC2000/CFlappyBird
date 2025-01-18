#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "pipe.h"

typedef struct Level {
	Texture2D texture;
	Pipe** pipes;
	size_t maxPipes;
	size_t pipeCount;
} Level;

void LevelInit(Level* l, Texture2D texture);
void LevelSpawnPipe(Level* l, Vector2 pos, _Bool flipped);

#endif
