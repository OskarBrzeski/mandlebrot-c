#include "raylib.h"

#include <signal.h>
#include <stdio.h>

bool quitGame = false;
const int windowWidth = 800;
const int windowHeight = 600;

int
main(void) {
	InitWindow(windowWidth, windowHeight, "Cilantro");
	SetWindowMonitor(0);

	while (!WindowShouldClose() && !quitGame) {
		BeginDrawing();
		ClearBackground(BLACK);

		Color pixels[windowWidth * windowHeight];
		for (int i = 0; i < windowHeight; i++) {
			for (int j = 0; j < windowWidth; j++) {
				pixels[i * windowWidth + j] = (Color){
				    .r = (float)j / windowWidth * 255.0,
				    .g = (float)i / windowHeight * 255.0,
				    .b = 0,
				    .a = 255};
			}
		}

		Image image = {
		    .data = pixels,
		    .width = windowWidth,
		    .height = windowHeight,
		    .format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,
		    .mipmaps = 1,
		};

		Texture2D texture = LoadTextureFromImage(image);

		DrawTexture(texture, 0, 0, WHITE);

		DrawFPS(5, 5);
		EndDrawing();

		UnloadTexture(texture);
	}

	CloseWindow();

	return 0;
}
