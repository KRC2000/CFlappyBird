#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "globals.h"
#include "raylib.h"

#include "level.h"
#include "bird.h"
#include "pipe.h"

#define MAX_PIPES 10

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void) {
	// Initialization
	//--------------------------------------------------------------------------------------
	const float gravity = 200;

	Globals* G = getGlobals();
	InitWindow(G->screenWidth, G->screenHeight,
			   "raylib [core] example - basic window");

	SetTargetFPS(0);  // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------
	Texture2D texture = LoadTexture("sheet.png");
	Level* level = malloc(sizeof(Level));
	LevelInit(level, texture);

	LevelSpawnPipe(level, (Vector2){150, 150}, false);
	LevelSpawnPipe(level, (Vector2){300, 150}, true);
	LevelSpawnPipe(level, (Vector2){200, 500}, false);

	Bird* b = BirdNew((Vector2){10, 0}, 150, texture, (Vector2){8, 8});

	// Main game loop
	while (!WindowShouldClose())  // Detect window close button or ESC key
	{
		float delta = GetFrameTime();
		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		if (IsKeyPressed(KEY_SPACE)) {
			b->speed = (Vector2){0, -b->flapStrength};
		}

		for (size_t i = 0; i < level->pipeCount; i++) {
			// Move pipes left
			level->pipes[i]->pos.x -= G->speed * delta;

			// Remove pipes that out of the frame
			if (level->pipes[i]->pos.x + level->pipes[i]->texBodySource.width * level->pipes[i]->scale < 0) {
				free(level->pipes[i]);
				// If deleted pipe is not the last in the array - shift elements on the right
				// from deleted element one index to the left to fill emptied index
				if (level->pipeCount != level->maxPipes - 1)
					memcpy(&level->pipes[i], &level->pipes[i+1], sizeof(Pipe*) * (level->maxPipes - 1 - i ));
				level->pipeCount--;
			}
		}


		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(BLACK);

		for (size_t i = 0; i < level->pipeCount; i++) {
			PipeDraw(level->pipes[i]);
		}

		BirdUpdate(b, gravity, delta);
		BirdDraw(b);

		DrawText("Congrats! You created your first window!", 190, 200, 20,
				 LIGHTGRAY);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();	// Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
