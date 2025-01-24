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

	char ch = str[0];
	for (size_t i = 1; ch != '\0'; i++) {
		int n = ch - 48;  // Smart way to convert numerical char into a number
						  // it represents

		// TODO: Fix me! I calculate offsets incorrectly. Look at `UiGetNumDrawSize` to see how its done.
		Rectangle dest = {
			pos.x + (i - 1) * (ui->glyphs[n].source.width * getGlobals()->scale + spacing), pos.y,
			ui->glyphs[n].source.width * getGlobals()->scale, ui->glyphs[n].source.height * getGlobals()->scale};
		DrawTexturePro(ui->texture, ui->glyphs[n].source, dest, (Vector2){0, 0},
					   0, WHITE);
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

	float fontSize = 0;
	char* text = 0;
	Vector2 textSize = {0, 0};
	Vector2 pos = {0, 0};

	if (G->state == PLAY) {
		Vector2 size = UiGetNumDrawSize(ui, G->score, 15);
		Vector2 pos = {(float)G->screenWidth / 2 - size.x / 2, G->screenHeight - G->bottomPadding};
		UiDrawNum(ui, G->score, pos, 15);
	}

	if (G->state == DEATH) {
		fontSize = 80;
		text = "You crashed!";
		textSize = MeasureTextEx(ui->font, text, fontSize, 0);
		pos = (Vector2){(float)G->screenWidth / 2 - textSize.x / 2,
						(float)G->screenHeight / 2 - fontSize};
		DrawTextEx(ui->font, text, pos, fontSize, 0, GREEN);

		text = "[R] Restart";
		textSize = MeasureTextEx(ui->font, text, fontSize, 0);
		pos = (Vector2){(float)G->screenWidth / 2 - textSize.x / 2,
						pos.y + fontSize};
		DrawTextEx(ui->font, text, pos, fontSize, 0, GREEN);
	}

	if (G->state == MENU) {
		// fontSize = 80;
		// text = "Flappy Bird";
		// textSize = MeasureTextEx(ui->font, text, fontSize, 0);
		// pos = (Vector2){(float)G->screenWidth / 2 - textSize.x / 2,
		//			   (float)G->screenHeight / 2 - fontSize};
		// DrawTextEx(ui->font, text, pos, fontSize, 0, RAYWHITE);

		// text = "[P] Play";
		// textSize = MeasureTextEx(ui->font, text, 80, 0);
		// pos = (Vector2){(float)G->screenWidth / 2 - textSize.x / 2,
		//			   pos.y + fontSize};
		// DrawTextEx(ui->font, text, pos, fontSize, 0, RAYWHITE);

		Rectangle dest = {(float)G->screenWidth / 2 -
							  (ui->startMsgSource.width * G->scale) / 2,
						  (float)G->screenHeight / 2 -
							  (ui->startMsgSource.height * G->scale) / 2,
						  ui->startMsgSource.width * G->scale,
						  ui->startMsgSource.height * G->scale};
		DrawTexturePro(ui->texture, ui->startMsgSource, dest, (Vector2){0, 0},
					   0, WHITE);

		dest = (Rectangle){(float)G->screenWidth / 2 -
							  (ui->bestMsgSource.width * G->scale) / 2,
						  (float)G->screenHeight / 2 -
							  (ui->bestMsgSource.height * G->scale) / 2 + 100,
						  ui->bestMsgSource.width * G->scale,
						  ui->bestMsgSource.height * G->scale};

		DrawTexturePro(ui->texture, ui->bestMsgSource, dest, (Vector2){0, 0},
					   0, WHITE);

		UiDrawNum(ui, G->bestScore, (Vector2){0, 0}, 15);
	}
}
