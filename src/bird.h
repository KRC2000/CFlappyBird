#ifndef BIRD_H
#define BIRD_H

#include "raylib.h"

typedef struct Bird {
	Vector2 pos;
	Vector2 speed;
	float flapStrength;
	Rectangle textureSource;
	Texture2D texture;
	Vector2 scale;
	float radius;
} Bird;

Bird* BirdNew(Vector2 pos, float flapStrength, Texture2D texture, Vector2 scale);
void BirdDraw(Bird* b);
void BirdUpdate(Bird* b, float gravity, float delta);

#endif	// !BIRD
