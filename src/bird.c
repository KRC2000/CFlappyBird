#include "bird.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "globals.h"
#include "raylib.h"
#include "raymath.h"

Bird* BirdNew(Vector2 pos,
			  float flapStrength,
			  Texture2D texture,
			  Vector2 scale) {
	Bird* b = malloc(sizeof(Bird));
	b->texture = texture;
	b->textureSource = (Rectangle){34, 0, 12, 10};
	b->pos = pos;
	b->speed = (Vector2){0, 0};
	b->scale = (Vector2){8, 8};
	b->flapStrength = flapStrength;
	b->radius = fminf(b->textureSource.width / 2 * b->scale.x, b->textureSource.height / 2 * b->scale.y);
	return b;
}

void BirdDraw(Bird* b) {
	Vector2 destSize = {b->textureSource.width * b->scale.x,
						b->textureSource.height * b->scale.y};
	Rectangle dest = {b->pos.x, b->pos.y, destSize.x, destSize.y};
	Vector2 origin = {destSize.x / 2, destSize.y / 2};
	float rot = Clamp(b->speed.y / 10, -90, 90);
	DrawTexturePro(b->texture, b->textureSource, dest, origin, rot, WHITE);
	if (getGlobals()->drawColliders) DrawCircleLines(b->pos.x, b->pos.y, b->radius, PINK);

	char strx[10];
	char stry[10];
	snprintf(strx, sizeof(strx), "%.4f", b->speed.x);
	snprintf(stry, sizeof(stry), "%.4f", b->speed.y);

	char* a = strcat(strcat(strx, "\n"), stry);
	DrawText(a, b->pos.x + destSize.x, b->pos.y, 24, GREEN);
}

void BirdUpdate(Bird* b, float gravity, float delta) {
	b->speed.y += gravity * delta;
	b->pos = Vector2Add(b->pos, Vector2Scale(b->speed, delta));
}
