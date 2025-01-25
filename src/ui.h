#ifndef UI_H
#define UI_H

#include "raylib.h"

#include "glyph.h"

typedef struct Ui {
	Texture2D texture;
	Rectangle startMsgSource;
	Rectangle bestMsgSource;
	Rectangle overMsgSource;
	Font font;
	Glyph* glyphs;
} Ui;

Ui* UiNew(Texture2D texture);
void UiDrawNum(Ui* ui, unsigned int num, Vector2 pos, int spacing);
void UiDraw(Ui* ui);

#endif
