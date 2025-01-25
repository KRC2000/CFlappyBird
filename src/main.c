#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "raylib.h"

#include "bird.h"
#include "level.h"
#include "pipe.h"
#include "ui.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
	// Initialization
	//--------------------------------------------------------------------------------------
	setGlobals();
	Globals* G = getGlobals();
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(G->screenWidth, G->screenHeight,
			   "raylib [core] example - basic window");

	SetTargetFPS(0);  // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	Texture2D texture = LoadTexture("sheet.png");
	Level* level = malloc(sizeof(Level));
	LevelInit(level, texture);

	Bird* b = BirdNew(G->birdStartPos, 300, texture);

	Ui* ui = UiNew(texture);
	ui->font = LoadFontEx("ArcadeClassic.ttf", 120, 0, 250);

	// Main game loop
	while (!WindowShouldClose())  // Detect window close button or ESC key
	{
		float delta = GetFrameTime();
		G->time += delta;
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------
		if (IsWindowResized()) {
			G->screenWidth = GetScreenWidth();
			G->screenHeight = GetScreenHeight();
		}

		LevelProcess(level, delta);


		if (G->state == MENU) {
			if (IsKeyPressed(KEY_ENTER)) {
				G->state = PLAY;
			}
		}

		if (G->state == PLAY) {
			BirdUpdate(b, G->gravity, delta);
			bool pipeCol = false;
			bool ceilingCol = CheckCollisionCircleRec(
				b->pos, b->radius, (Rectangle){0, -100, G->screenWidth, 100});
			bool floorCol = CheckCollisionCircleRec(
				b->pos, b->radius,
				(Rectangle){0, G->screenHeight - G->bottomPadding,
							G->screenWidth, G->bottomPadding});
			for (size_t i = 0; i < level->pipeCount; i++) {
				pipeCol = CheckCollisionCircleRec(
					b->pos, b->radius, PipeGetBodyRect(level->pipes[i]));

				if (pipeCol) break;
			}

			if (pipeCol || ceilingCol || floorCol) {
				G->state = DEATH;
				if(G->score > G->bestScore) {
					ui->best = true;
					writeBestScore(getGlobals()->score);
				}
			}
		}

		if (G->state == DEATH) {
			if (IsKeyPressed(KEY_ENTER)) {
				G->state = PLAY;
				LevelReset(level);
				BirdReset(b);
				G->score = 0;
				ui->flapped = false;
				ui->best = false;
			}
		}

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		// ParallaxLayerDraw(pl);
		LevelDraw(level);
		if (G->state != MENU)
			BirdDraw(b);
		UiDraw(ui);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();	// Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
