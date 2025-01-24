#ifndef PARALLAX_H
#define PARALLAX_H

#include "raylib.h"

typedef struct ParallaxLayer {
	Texture2D upper;
	Rectangle source;
	Texture2D texture;
	float posY;
	float scroll;
} ParallaxLayer;

ParallaxLayer ParallaxLayerNew(float posY, Rectangle source, Texture2D tex);
void ParallaxLayerProcess(ParallaxLayer* pl, float delta);
void ParallaxLayerDraw(ParallaxLayer pl);

#endif
