#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	Globals* G = getGlobals();
	InitWindow(G->screenWidth, G->screenHeight,
			   "raylib [core] example - basic window");

	SetTargetFPS(0);  // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------
	Texture2D texture = LoadTexture("sheet.png");
	Level* level = malloc(sizeof(Level));
	LevelInit(level, texture);

	Bird* b = BirdNew((Vector2){10, 0}, 300, texture, (Vector2){8, 8});

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

		LevelProcess(level, delta);

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		for (size_t i = 0; i < level->pipeCount; i++) {
			PipeDraw(level->pipes[i]);
		}

		BirdUpdate(b, G->gravity, delta);
		BirdDraw(b);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();	// Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
