#ifndef UI_H
#define UI_H

#include "raylib.h"

#include "glyph.h"

typedef struct {
	Texture2D texture;
	Rectangle startMsgSource;
	Rectangle bestMsgSource;
	Rectangle newBestMsgSource;
	Rectangle overMsgSource;
	Rectangle enterSource;
	Rectangle spaceSource;
	Rectangle scoreSource;
	Glyph* glyphs;
	float numCharSpacing;
	bool flapped;
	bool best;
} Ui;

typedef struct {
	Vector2 offset;
	Rectangle rect;
} OffsetRect;

Ui* UiNew(Texture2D texture);
static void UiDrawNum(Ui* ui, unsigned int num, Vector2 pos, int spacing);
static Vector2 UiGetNumDrawSize(Ui* ui, unsigned int num, int spacing);
static void UiDrawCenterBottomNum(Ui* ui, unsigned int num, float margin);

/*
 * This function draws a column of elements in the center of the screen.
 * It supports 3 types: 'OffsetRect', 'Rectangle' and 'unsigned int'.
 * Pass texture source rectangle and offset as 'OffsetRect' to draw a
 * texture segment with an offset. Useful to add some animation by
 * changing offset.
 * Pass texture source rectangle as 'Rectangle' to draw a texture segment.
 * Pass number as 'unsigned int' to draw a number.
 * 'layout' parameter string should represent types of passed elements.
 * E.g. layout == "rrRu" tells this function that you passed 2 'Rectangle's
 * 1 'OffsetRect' and an 'unsigned int' to draw in a column.
 * GLobal scaling applies to the drawings.
 */
static void UiDrawColumnCentered(Ui* ui,
								 float padding,
								 const char* layout,
								 ...);
void UiDraw(Ui* ui);

#endif
