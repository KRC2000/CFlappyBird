#ifndef LEVEL_H
#define LEVEL_H

#include <stdio.h>
#include <raylib.h>
#include "pipe.h"

typedef struct Level {
	Texture2D texture;
	Pipe** pipes;
	size_t maxPipes;
	size_t pipeCount;
	float pipeSpawnTimer;
	float pipeSpawnFreq;
} Level;

void LevelInit(Level* l, Texture2D texture);
void LevelSpawnPipe(Level* l, float yPos, _Bool flipped);
void LevelSpawnPipeGate(Level* l, float yPos, float gapSize);
void LevelProcess(Level* l, float delta);

#endif
