#include "globals.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

static Globals globals;

void setGlobals() {
	globals.drawColliders = true;
	globals.state = MENU;
	globals.scale = 8;
	globals.time = 0;
	globals.screenWidth = 450;
	globals.screenHeight = 800;
	globals.score = 0;
	globals.speed = 100;
	globals.gravity = 800;
	globals.bottomPadding = 100;
	globals.birdStartPos = (Vector2){100, 100};


	FILE* f = fopen("score", "rb");
	if (!f) {
		printf("WARNING: GAME: Couldn't read best score. Failed to open file. Maybe its first launch?\n");
		return;
	}

	unsigned int num;
	size_t read = fread(&num, sizeof(unsigned int), 1, f);
	if (read != 1) {
		printf("ERROR: GAME: Couldn't read best score. Corrupted data.\n");
		fclose(f);
		return;
	}

	globals.bestScore = num;
	printf("INFO: GAME: Best score read: %u.\n", globals.bestScore);

	fclose(f);
}

Globals* getGlobals() {
    return &globals;
}

void writeBestScore(unsigned int s) {
	if (s <= getGlobals()->bestScore) return;

	FILE* f = fopen("score", "wb");
	if (!f) {
		printf("INFO: GAME: Couldn't write best score. Failed to write file.\n");
		return;
	}

	fwrite(&getGlobals()->score, sizeof(unsigned int), 1, f);
	printf("INFO: GAME: Best score written: %u.\n", globals.score);
	fclose(f);
}

