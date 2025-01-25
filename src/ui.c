#include "ui.h"

#include <stdio.h>
#include <stdlib.h>
#include "globals.h"
#include "raylib.h"

Ui* UiNew(Texture2D texture) {
	Ui* ui = malloc(sizeof(Ui));
	ui->texture = texture;
	ui->startMsgSource = (Rectangle){135, 4, 29, 15};
	ui->bestMsgSource = (Rectangle){131, 72, 23, 7};
	ui->overMsgSource = (Rectangle){135, 20, 26, 15};

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

Vector2 UiGetNumDrawSize(Ui* ui, unsigned int num, int spacing) {
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

void UiDraw(Ui* ui) {
	Globals* G = getGlobals();
	Vector2 center = {(float)G->screenWidth / 2, (float)G->screenHeight / 2};
	Vector2 scoreSize = UiGetNumDrawSize(ui, G->score, 15);
	Vector2 scorePos = {center.x - scoreSize.x / 2,
						G->screenHeight - G->bottomPadding};

	if (G->state == PLAY) {
		UiDrawNum(ui, G->score, scorePos, 15);
	}

	if (G->state == DEATH) {
		Rectangle dest = {center.x - (ui->overMsgSource.width * G->scale) / 2,
						  center.y - (ui->overMsgSource.height * G->scale) / 2,
						  ui->overMsgSource.width * G->scale,
						  ui->overMsgSource.height * G->scale};
		DrawTexturePro(ui->texture, ui->overMsgSource, dest, (Vector2){0, 0}, 0,
					   WHITE);
		UiDrawNum(ui, G->score, scorePos, 15);
	}

	if (G->state == MENU) {
		Rectangle dest = {center.x - (ui->startMsgSource.width * G->scale) / 2,
						  center.y - (ui->startMsgSource.height * G->scale) / 2,
						  ui->startMsgSource.width * G->scale,
						  ui->startMsgSource.height * G->scale};
		DrawTexturePro(ui->texture, ui->startMsgSource, dest, (Vector2){0, 0},
					   0, WHITE);

		dest = (Rectangle){center.x - (ui->bestMsgSource.width * G->scale) / 2,
						   dest.y + 200, ui->bestMsgSource.width * G->scale,
						   ui->bestMsgSource.height * G->scale};

		DrawTexturePro(ui->texture, ui->bestMsgSource, dest, (Vector2){0, 0}, 0,
					   WHITE);

		Vector2 size = UiGetNumDrawSize(ui, G->bestScore, 15);
		Vector2 pos = (Vector2){center.x - size.x / 2, dest.y + 80};
		UiDrawNum(ui, G->bestScore, pos, 15);
	}
}
