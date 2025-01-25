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
			  Texture2D texture) {
	Bird* b = malloc(sizeof(Bird));
	b->texture = texture;
	b->textureSource = (Rectangle){34, 0, 12, 10};
	b->pos = pos;
	b->speed = (Vector2){0, 0};
	b->flapStrength = flapStrength;
	b->radius = fminf(b->textureSource.width / 2 * getGlobals()->scale, b->textureSource.height / 2 * getGlobals()->scale);
	return b;
}

void BirdUpdate(Bird* b, float gravity, float delta) {
	b->speed.y += gravity * delta;
	b->pos = Vector2Add(b->pos, Vector2Scale(b->speed, delta));

	if (IsKeyPressed(KEY_SPACE)) {
		b->speed = (Vector2){0, -b->flapStrength};
		getGlobals()->score += 1;
	}
}

void BirdDraw(Bird* b) {
	Vector2 destSize = {b->textureSource.width * getGlobals()->scale,
						b->textureSource.height * getGlobals()->scale};
	Rectangle dest = {b->pos.x, b->pos.y, destSize.x, destSize.y};
	Vector2 origin = {destSize.x / 2, destSize.y / 2};
	float rot = Clamp(b->speed.y / 10, -90, 90);
	DrawTexturePro(b->texture, b->textureSource, dest, origin, rot, WHITE);
	if (getGlobals()->drawColliders) DrawCircleLines(b->pos.x, b->pos.y, b->radius, PINK);
}

void BirdReset(Bird* b) {
	b->pos = getGlobals()->birdStartPos;
	b->speed = (Vector2){0, 0};
}
