#include "level.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "parallax.h"
#include "raylib.h"

void LevelInit(Level* l, Texture2D texture) {
	l->pipeCount = 0;
	l->floorScroll = 0;
	l->maxPipes = 30;
	l->pipes = malloc(sizeof(Pipe*) * l->maxPipes);
	l->texture = texture;
	l->pipeSpawnTimer = 0;
	l->pipeSpawnFreq = 2.5;
	l->floorSource = (Rectangle){1, 20, 40, 7};

	// Generate a tiling texture for a floor
	RenderTexture2D rt =
		LoadRenderTexture(l->floorSource.width, l->floorSource.height);
	SetTextureWrap(rt.texture, TEXTURE_WRAP_MIRROR_REPEAT);
	BeginTextureMode(rt);
	DrawTextureRec(l->texture, l->floorSource, (Vector2){0, 0}, WHITE);
	EndTextureMode();
	l->floorTexture = rt.texture;

	// Initialize parallax layers
	l->pxLayersCount = 7;
	l->pxLayers = malloc(sizeof(ParallaxLayer) * l->pxLayersCount);
	float sh = getGlobals()->screenHeight;
	l->pxLayers[0] = ParallaxLayerNew(0, (Rectangle){1, 28, 40, 1}, texture);
	l->pxLayers[1] =
		ParallaxLayerNew(sh - 500, (Rectangle){42, 35, 40, 3}, texture);
	l->pxLayers[2] =
		ParallaxLayerNew(sh - 350, (Rectangle){83, 39, 40, 3}, texture);
	l->pxLayers[3] =
		ParallaxLayerNew(sh - 250, (Rectangle){124, 43, 40, 3}, texture);
	l->pxLayers[4] =
		ParallaxLayerNew(sh - 200, (Rectangle){1, 61, 40, 3}, texture);
	l->pxLayers[5] =
		ParallaxLayerNew(sh - 150, (Rectangle){42, 63, 40, 3}, texture);
	l->pxLayers[6] =
		ParallaxLayerNew(sh - 100, (Rectangle){83, 65, 40, 5}, texture);
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
	if (getGlobals()->state == DEATH) return;
	for (size_t i = 0; i < l->pxLayersCount; i++)
		ParallaxLayerProcess(&l->pxLayers[i], delta);

	if (getGlobals()->state != PLAY) return;

	l->floorScroll += getGlobals()->speed * delta / getGlobals()->scale;

	l->pipeSpawnTimer += delta;
	if (l->pipeSpawnTimer >= l->pipeSpawnFreq) {
		float gap = 300;
		int r = GetRandomValue(0, getGlobals()->screenHeight - getGlobals()->bottomPadding - gap);
		LevelSpawnPipeGate(l, r, gap);
		l->pipeSpawnTimer = 0;
	}

	int pipeToFreeIdx = -1;
	for (size_t i = 0; i < l->pipeCount; i++) {
		// Move pipes left
		l->pipes[i]->pos.x -= getGlobals()->speed * delta;

		// Find first pipe that is out of screen bounds and should be deleted
		if (l->pipes[i]->pos.x +
				l->pipes[i]->texBodySource.width * getGlobals()->scale <
			0)
			pipeToFreeIdx = i;
	}

	// If there is a pipe to delete - delete it
	if (pipeToFreeIdx != -1) {
		free(l->pipes[pipeToFreeIdx]);
		// If deleted pipe is not the last in the array - shift elements on
		// the right from deleted element one index to the left to fill
		// emptied index
		if (l->pipeCount != l->maxPipes - 1)
			memcpy(&l->pipes[pipeToFreeIdx], &l->pipes[pipeToFreeIdx + 1],
				   sizeof(Pipe*) * (l->maxPipes - 1 - pipeToFreeIdx));
		l->pipeCount--;
	}
}

void LevelReset(Level* l) {
	// Delete all pipes
	for (size_t i = 0; i < l->pipeCount; i++) {
		free(l->pipes[i]);
	}
	l->pipeCount = 0;
	l->pipeSpawnTimer = 0;
}

void LevelDraw(Level* l) {
	for (size_t i = 0; i < l->pxLayersCount; i++) {
		ParallaxLayerDraw(l->pxLayers[i]);
	}

	for (size_t i = 0; i < l->pipeCount; i++)
		PipeDraw(l->pipes[i]);

	float factor = getGlobals()->screenWidth /
				   (l->floorTexture.width * getGlobals()->scale);
	Rectangle source = {l->floorScroll, 0, l->floorTexture.width * factor,
						l->floorSource.height};
	Rectangle dest = {
		0, getGlobals()->screenHeight - getGlobals()->bottomPadding,
		getGlobals()->screenWidth, l->floorSource.height * getGlobals()->scale};
	DrawTexturePro(l->floorTexture, source, dest, (Vector2){0, 0}, 0, WHITE);
}
