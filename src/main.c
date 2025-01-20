#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "raylib.h"

#include "level.h"
#include "bird.h"
#include "pipe.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
	// Initialization
	//--------------------------------------------------------------------------------------
	setGlobals();
	Globals* G = getGlobals();

	InitWindow(G->screenWidth, G->screenHeight,
			   "raylib [core] example - basic window");

	SetTargetFPS(30);  // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------
	Texture2D texture = LoadTexture("sheet.png");
	Level* level = malloc(sizeof(Level));
	LevelInit(level, texture);

	Bird* b = BirdNew((Vector2){50, 0}, 300, texture, (Vector2){8, 8});

	// Main game loop
	while (!WindowShouldClose())  // Detect window close button or ESC key
	{
		float delta = GetFrameTime();
		G->time += delta;
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		if (IsKeyPressed(KEY_SPACE)) {
			b->speed = (Vector2){0, -b->flapStrength};
		}


		if (G->state == PLAY) LevelProcess(level, delta);

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);


		for (size_t i = 0; i < level->pipeCount; i++)
			PipeDraw(level->pipes[i]);

		if (G->state == PLAY) {
			BirdUpdate(b, G->gravity, delta);
			for (size_t i = 0; i < level->pipeCount; i++) {
				bool col = CheckCollisionCircleRec(b->pos, b->radius, PipeGetBodyRect(level->pipes[i]));
				if (col) G->state = DEATH;
			}
		}

		BirdDraw(b);

		if (G->state == DEATH) {
			DrawText("You crashed!", 0, 0, 40, GREEN);
			DrawText("Score: 2349", 0, 40, 40, GREEN);
		}

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();	// Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
