#include "level.h"


void LevelInit(Level* l, Texture2D texture) {
	l->pipeCount = 0;
	l->maxPipes = 10;
	l->pipes = malloc(sizeof(*l->pipes) * l->maxPipes);
	l->texture = texture;
}

void LevelSpawnPipe(Level* l, Vector2 pos, _Bool flipped) {
	Pipe* p = PipeNew(l->texture, pos, (Rectangle){3, 9, 12, 10},
				(Rectangle){1, 1, 16, 7}, (Vector2){-2, -2}, flipped);
	l->pipes[l->pipeCount] = p;
	l->pipeCount++;
}
