#ifndef PIPE
#define PIPE

#include "raylib.h"

typedef struct Pipe {
	Texture2D texture;
	Texture2D pipeBodyTex;
	float scale;
	_Bool flipped;
	Rectangle texBodySource;
	Rectangle texHatSource;
	Vector2 pos;
	Vector2 hatOffset;
} Pipe;

static Texture2D pipeBodyTex;

Pipe* PipeNew(Texture2D tex,
			  Vector2 pos,
			  Rectangle texBodySource,
			  Rectangle texHatSource,
			  Vector2 hatOffset,
			  _Bool flipped);
Rectangle PipeGetBodyRect(Pipe* p);
void PipeDraw(Pipe* p);

#endif	// !PIPE
