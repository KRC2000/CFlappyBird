#include "ui.h"

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "globals.h"
#include "raylib.h"

Ui* UiNew(Texture2D texture) {
	Ui* ui = malloc(sizeof(Ui));
	ui->flapped = false;
	ui->best = false;
	ui->texture = texture;
	ui->numCharSpacing = 15;
	ui->startMsgSource = (Rectangle){135, 4, 29, 15};
	ui->bestMsgSource = (Rectangle){131, 72, 23, 7};
	ui->newBestMsgSource = (Rectangle){131, 64, 27, 15};
	ui->overMsgSource = (Rectangle){135, 20, 26, 15};
	ui->enterSource = (Rectangle){206, 25, 18, 17};
	ui->spaceSource = (Rectangle){202, 1, 22, 8};
	ui->scoreSource = (Rectangle){102, 26, 29, 7};

	ui->glyphs = malloc(sizeof(Glyph) * 9);
	ui->glyphs[0] = (Glyph){'0', (Rectangle){172, 1, 5, 7}};
	ui->glyphs[1] = (Glyph){'1', (Rectangle){179, 1, 3, 7}};
	ui->glyphs[2] = (Glyph){'2', (Rectangle){184, 1, 5, 7}};
	ui->glyphs[3] = (Glyph){'3', (Rectangle){190, 1, 5, 7}};
	ui->glyphs[4] = (Glyph){'4', (Rectangle){196, 1, 5, 7}};
	ui->glyphs[5] = (Glyph){'5', (Rectangle){172, 9, 5, 7}};
	ui->glyphs[6] = (Glyph){'6', (Rectangle){178, 9, 5, 7}};
	ui->glyphs[7] = (Glyph){'7', (Rectangle){184, 9, 5, 7}};
	ui->glyphs[8] = (Glyph){'8', (Rectangle){190, 9, 5, 7}};
	ui->glyphs[9] = (Glyph){'9', (Rectangle){196, 9, 5, 7}};

	return ui;
}

void UiDrawNum(Ui* ui, unsigned int num, Vector2 pos, int spacing) {
	char* str = malloc(10);
	sprintf(str, "%u", num);

	float offsetAccum = 0;
	char ch = str[0];
	for (size_t i = 1; ch != '\0'; i++) {
		int n = ch - 48;  // Smart way to convert numerical char into a number
						  // it represents

		Rectangle dest = {pos.x + offsetAccum, pos.y,
						  ui->glyphs[n].source.width * getGlobals()->scale,
						  ui->glyphs[n].source.height * getGlobals()->scale};
		DrawTexturePro(ui->texture, ui->glyphs[n].source, dest, (Vector2){0, 0},
					   0, WHITE);
		offsetAccum +=
			ui->glyphs[n].source.width * getGlobals()->scale + spacing;
		ch = str[i];
	}
}

static void UiDrawCenterBottomNum(Ui* ui, unsigned int num, float margin) {
	Vector2 screenCenter = {(float)getGlobals()->screenWidth / 2,
							(float)getGlobals()->screenHeight / 2};

	Vector2 size = UiGetNumDrawSize(ui, num, ui->numCharSpacing);
	Vector2 pos = {screenCenter.x - size.x / 2,
				   getGlobals()->screenHeight - size.y - margin};
	UiDrawNum(ui, num, pos, ui->numCharSpacing);
}

static Vector2 UiGetNumDrawSize(Ui* ui, unsigned int num, int spacing) {
	Vector2 size = {0};

	char* str = malloc(10);
	sprintf(str, "%u", num);

	char ch = str[0];
	size_t charCount = 0;
	for (size_t i = 1; ch != '\0'; i++) {
		charCount++;
		int n = ch - 48;  // Smart way to convert numerical char into a number
						  // it represents

		size.x += ui->glyphs[n].source.width * getGlobals()->scale;
		if (size.y < ui->glyphs[n].source.height * getGlobals()->scale)
			size.y = ui->glyphs[n].source.height * getGlobals()->scale;

		ch = str[i];
	}

	if (charCount >= 2)
		size.x += spacing * (charCount - 1);

	return size;
}

static void UiDrawColumnCentered(Ui* ui,
								 float padding,
								 const char* layout,
								 ...) {
	va_list ap;
	va_start(ap, layout);

	Globals* G = getGlobals();
	Vector2 screenCenter = {(float)G->screenWidth / 2,
							(float)G->screenHeight / 2};
	float startY;
	size_t count = 0;
	float height = 0;

	// Pass 1: Calculate the height of the resulting column
	for (size_t i = 0; i < strlen(layout); i++) {
		if (layout[i] == 'r') {
			Rectangle source = va_arg(ap, Rectangle);
			height += source.height * G->scale;
			count++;
		}
		if (layout[i] == 'R') {
			OffsetRect offRect = va_arg(ap, OffsetRect);
			height += offRect.rect.height * G->scale;
			count++;
		}
		if (layout[i] == 'u') {
			unsigned int num = va_arg(ap, unsigned int);
			height += UiGetNumDrawSize(ui, num, ui->numCharSpacing).y;
			count++;
		}
	}

	if (count > 1)
		height += padding * (count - 1);
	startY = screenCenter.y - height / 2;

	va_end(ap);

	// Pass 2: Draw each element
	va_start(ap, layout);
	for (size_t i = 0; i < strlen(layout); i++) {
		if (layout[i] == 'r') {
			Rectangle source = va_arg(ap, Rectangle);
			Vector2 destSize = {source.width * getGlobals()->scale,
								source.height * getGlobals()->scale};
			Rectangle dest = {screenCenter.x - destSize.x / 2, startY,
							  destSize.x, destSize.y};
			DrawTexturePro(ui->texture, source, dest, (Vector2){0}, 0, WHITE);
			startY += destSize.y + padding;
		}
		if (layout[i] == 'R') {
			OffsetRect offRect = va_arg(ap, OffsetRect);
			Vector2 destSize = {offRect.rect.width * getGlobals()->scale,
								offRect.rect.height * getGlobals()->scale};
			Rectangle dest = {
				screenCenter.x - destSize.x / 2 + offRect.offset.x,
				startY + offRect.offset.y, destSize.x, destSize.y};
			DrawTexturePro(ui->texture, offRect.rect, dest, (Vector2){0}, 0,
						   WHITE);
			startY += destSize.y + padding;
		}
		if (layout[i] == 'u') {
			unsigned int num = va_arg(ap, unsigned int);
			Vector2 destSize = UiGetNumDrawSize(ui, num, ui->numCharSpacing);
			Vector2 pos = {screenCenter.x - destSize.x / 2, startY};
			UiDrawNum(ui, num, pos, ui->numCharSpacing);
			startY += destSize.y + padding;
		}
	}
}

void UiDraw(Ui* ui) {
	Globals* G = getGlobals();
	float waveRangeFactor = 10;
	float waveSpeed = 10;
	float wave = sin(G->time * waveSpeed) * waveRangeFactor;

	if (G->state == PLAY) {
		UiDrawCenterBottomNum(ui, G->score, 50);

		if (IsKeyPressed(KEY_SPACE))
			ui->flapped = true;
		if (!ui->flapped) {
			OffsetRect offRect = {(Vector2){0, wave}, ui->spaceSource};
			UiDrawColumnCentered(ui, 0, "R", offRect);
		}
	}

	if (G->state == DEATH) {
		OffsetRect enter = {(Vector2){0, wave}, ui->enterSource};
		UiDrawColumnCentered(ui, 30, "ruRr",
							 ui->best ? ui->newBestMsgSource : ui->scoreSource,
							 G->score, enter, ui->overMsgSource);
	}

	if (G->state == MENU) {
		OffsetRect enter = {(Vector2){0, wave}, ui->enterSource};
		UiDrawColumnCentered(ui, 30, "rRru", ui->startMsgSource, enter,
							 ui->bestMsgSource, G->bestScore);
	}

	if (G->drawFps) {
		int fps = GetFPS();
		char str[11];
		sprintf(str, "%d", fps);
		DrawText(str, 0, 0, 30, BLACK);
	}
}
