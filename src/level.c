#include "level.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "raylib.h"

void LevelInit(Level* l, Texture2D texture) {
	l->pipeCount = 0;
	l->maxPipes = 30;
	l->pipes = malloc(sizeof(Pipe*) * l->maxPipes);
	l->texture = texture;
	l->pipeSpawnTimer = 0;
	l->pipeSpawnFreq = 2.5;
}

void LevelSpawnPipe(Level* l, float yPos, _Bool flipped) {
	if (l->pipeCount == l->maxPipes) {
		printf("Couldn't spawn pipe. maxPipes reached.");
		return;
	}
	Pipe* p = PipeNew(l->texture, (Vector2){getGlobals()->screenWidth, yPos},
					  (Rectangle){3, 9, 12, 10}, (Rectangle){1, 1, 16, 7},
					  (Vector2){-2, -2}, flipped);
	l->pipes[l->pipeCount] = p;
	l->pipeCount++;
}

void LevelSpawnPipeGate(Level* l, float yPos, float gapSize) {
	LevelSpawnPipe(l, yPos, true);
	LevelSpawnPipe(l, yPos + gapSize, false);
}

void LevelProcess(Level* l, float delta) {
	Globals* G = getGlobals();

	l->pipeSpawnTimer += delta;
	if (l->pipeSpawnTimer >= l->pipeSpawnFreq) {
		float gap = 300;
		int r = GetRandomValue(100, G->screenHeight - 100 - gap);
		LevelSpawnPipeGate(l, r, gap);
		l->pipeSpawnTimer = 0;
	}

	for (size_t i = 0; i < l->pipeCount; i++) {
		// Move pipes left
		l->pipes[i]->pos.x -= G->speed * delta;

		// Remove pipes that out of the frame
		if (l->pipes[i]->pos.x +
				l->pipes[i]->texBodySource.width * l->pipes[i]->scale <
			0) {
			free(l->pipes[i]);
			// If deleted pipe is not the last in the array - shift elements on
			// the right from deleted element one index to the left to fill
			// emptied index
			if (l->pipeCount != l->maxPipes - 1)
				memcpy(&l->pipes[i], &l->pipes[i + 1],
					   sizeof(Pipe*) * (l->maxPipes - 1 - i));
			l->pipeCount--;
		}
	}
}
