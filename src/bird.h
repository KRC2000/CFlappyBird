#ifndef BIRD_H
#define BIRD_H

#include "raylib.h"

typedef struct Bird {
	Vector2 pos;
	Vector2 speed;
	float flapStrength;
	Rectangle textureSource;
	Texture2D texture;
	float radius;
} Bird;

Bird* BirdNew(Vector2 pos, float flapStrength, Texture2D texture);
void BirdUpdate(Bird* b, float gravity, float delta);
void BirdDraw(Bird* b);
void BirdReset(Bird* b);

#endif	// !BIRD
