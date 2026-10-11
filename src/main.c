#include "raylib.h"

bool quitGame = false;
int windowWidth = 1200;
int windowHeight = 900;

int
main(void) {
	InitWindow(windowWidth, windowHeight, "Cilantro");
	SetWindowMonitor(0);

	while (!WindowShouldClose() && !quitGame) {
		BeginDrawing();
		ClearBackground(BLACK);
		DrawFPS(5, 5);
		EndDrawing();
	}

	CloseWindow();

	return 0;
}
