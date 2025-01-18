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
	p->scale = 8;
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

void PipeDraw(Pipe* p) {
	float length;
	Rectangle bodyDest, hatDest;
	Rectangle hatSource = p->texHatSource;
	if (!p->flipped) {
		length = getGlobals()->screenHeight - p->pos.y;
		bodyDest = (Rectangle){p->pos.x, p->pos.y,
							   p->texBodySource.width * p->scale, length};
		hatDest = (Rectangle){p->pos.x + p->hatOffset.x * p->scale,
							  p->pos.y + p->hatOffset.y * p->scale,
							  p->texHatSource.width * p->scale,
							  p->texHatSource.height * p->scale};
	} else {
		length = p->pos.y;
		bodyDest =
			(Rectangle){p->pos.x, 0, p->texBodySource.width * p->scale, length};
		hatSource = (Rectangle){hatSource.x, hatSource.y, hatSource.width, -hatSource.height};
		hatDest = (Rectangle){p->pos.x + p->hatOffset.x * p->scale,
							  length - (p->texHatSource.height + p->hatOffset.y) * p->scale,
							  p->texHatSource.width * p->scale,
							  p->texHatSource.height * p->scale};
	}

	DrawTexturePro(p->texture, hatSource, hatDest, (Vector2){0, 0}, 0,
				   WHITE);
	DrawTexturePro(pipeBodyTex,
				   (Rectangle){0, 0, p->texBodySource.width, length}, bodyDest,
				   (Vector2){0, 0}, 0, WHITE);
}
