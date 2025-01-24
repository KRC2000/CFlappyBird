#include "parallax.h"
#include "globals.h"
#include "raylib.h"

ParallaxLayer ParallaxLayerNew(float posY, Rectangle source, Texture2D tex) {
	ParallaxLayer pl;
	pl.posY = posY;
	pl.source = source;
	pl.texture = tex;

	Rectangle upperSource = {source.x, source.y, source.width,
							 source.height - 1};
	RenderTexture2D rt =
		LoadRenderTexture(upperSource.width, upperSource.height);
	SetTextureWrap(rt.texture, TEXTURE_WRAP_MIRROR_REPEAT);
	BeginTextureMode(rt);
	DrawTextureRec(tex, upperSource, (Vector2){0, 0}, WHITE);
	EndTextureMode();
	pl.upper = rt.texture;

	return pl;
}

void ParallaxLayerProcess(ParallaxLayer* pl, float delta) {
	pl->scroll += delta * pl->posY / 45;
}

void ParallaxLayerDraw(ParallaxLayer pl) {
	float factor =
		getGlobals()->screenWidth / (pl.upper.width * getGlobals()->scale);
	// Flipped source height because RenderTexture produces flipped texture
	Rectangle source = {pl.scroll, 0, pl.upper.width * factor, -pl.upper.height};
	Rectangle dest = {0, pl.posY, getGlobals()->screenWidth,
					  pl.upper.height * getGlobals()->scale};
	DrawTexturePro(pl.upper, source, dest, (Vector2){0, 0}, 0, WHITE);

	source = (Rectangle){pl.source.x, pl.source.y + pl.source.height - 1,
						 pl.source.width, 1};
	dest = (Rectangle){0, dest.y + dest.height, getGlobals()->screenWidth,
					   getGlobals()->screenHeight - (dest.y + dest.height)};
	DrawTexturePro(pl.texture, source, dest, (Vector2){0, 0}, 0, WHITE);
}
