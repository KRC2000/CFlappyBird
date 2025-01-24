#include "pipe.h"

#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "raylib.h"

Pipe* PipeNew(Texture2D tex,
			  Vector2 pos,
			  Rectangle texBodySource,
			  Rectangle texHatSource,
			  Vector2 hatOffset,
			  _Bool flipped) {
	Pipe* p = malloc(sizeof(Pipe));

	p->pos = pos;
	p->texture = tex;
	p->flipped = flipped;
	p->texBodySource = texBodySource;
	p->texHatSource = texHatSource;
	p->hatOffset = hatOffset;

	// If not yet done - generate pipe body texture.
	// We create a new texture only containing the pipe body.
	// This way we can tile it. Tiling needed to render it 
	// at any length.
	if (pipeBodyTex.id != 0)
		return p;

	printf("GAME: Generating pipe body texture. Should be called once.\n");
	RenderTexture2D rt =
		LoadRenderTexture(texBodySource.width, texBodySource.height);
	SetTextureWrap(rt.texture, TEXTURE_WRAP_MIRROR_REPEAT);
	BeginTextureMode(rt);
	DrawTextureRec(tex, texBodySource, (Vector2){0, 0}, WHITE);
	EndTextureMode();
	pipeBodyTex = rt.texture;

	return p;
}

Rectangle PipeGetBodyRect(Pipe* p) {
	Rectangle rect;
	float length;
	if (!p->flipped) {
		length = getGlobals()->screenHeight - (p->pos.y + getGlobals()->bottomPadding);
		rect = (Rectangle){p->pos.x, p->pos.y,
							   p->texBodySource.width * getGlobals()->scale, length};
	} else {
		length = p->pos.y;
		rect = (Rectangle){p->pos.x, 0, p->texBodySource.width * getGlobals()->scale, length};
	}
	return rect;
}

void PipeDraw(Pipe* p) {
	float length;
	Rectangle hatDest;
	Rectangle hatSource = p->texHatSource;
	Rectangle bodyDest = PipeGetBodyRect(p);
	if (!p->flipped) {
		length = getGlobals()->screenHeight - p->pos.y;
		hatDest = (Rectangle){p->pos.x + p->hatOffset.x * getGlobals()->scale,
							  p->pos.y + p->hatOffset.y * getGlobals()->scale,
							  p->texHatSource.width * getGlobals()->scale,
							  p->texHatSource.height * getGlobals()->scale};
	} else {
		length = p->pos.y;
		hatSource = (Rectangle){hatSource.x, hatSource.y, hatSource.width, -hatSource.height};
		hatDest = (Rectangle){p->pos.x + p->hatOffset.x * getGlobals()->scale,
							  length - (p->texHatSource.height + p->hatOffset.y) * getGlobals()->scale,
							  p->texHatSource.width * getGlobals()->scale,
							  p->texHatSource.height * getGlobals()->scale};
	}

	DrawTexturePro(pipeBodyTex,
				   (Rectangle){0, 0, p->texBodySource.width, length}, bodyDest,
				   (Vector2){0, 0}, 0, WHITE);
	DrawTexturePro(p->texture, hatSource, hatDest, (Vector2){0, 0}, 0,
				   WHITE);

	if (getGlobals()->drawColliders) DrawRectangleLinesEx(bodyDest, 3, PINK);
}
