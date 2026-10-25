#include "raylib.h"

#include <stdio.h>

bool quitGame = false;
const int windowWidth = 400;
const int windowHeight = 400;

typedef struct {
	double real;
	double imag;
} Complex;

typedef struct {
	Texture2D texture;
	Complex origin;
	double zoom;
} State;

Color
get_color(int count) {
	Color colors[8] = {RED,  ORANGE, YELLOW, GREEN,
	                   BLUE, VIOLET, PURPLE, PINK};
	return colors[count % 8];
}

int
is_inside(Complex z) {
	return (z.real * z.real + z.imag * z.imag) <= 4;
}

Complex
xy_to_complex(int x, int y, Complex origin, double zoom) {
	double real =
	    (double)x / windowWidth * (4 + zoom) - (2 + zoom / 2) + origin.real;
	double imag = (double)y / windowHeight * (4 + zoom) - (2 + zoom / 2) +
	              origin.imag;
	return (Complex){.real = real, .imag = imag};
}

int
complex_equal(Complex a, Complex b) {
	return a.real == b.real && a.imag == b.imag;
}

Complex
complex_add(Complex a, Complex b) {
	return (Complex){.real = a.real + b.real, .imag = a.imag + b.imag};
}

Complex
complex_square(Complex z) {
	return (Complex){
	    .real = z.real * z.real - z.imag * z.imag,
	    .imag = 2 * z.real * z.imag,
	};
}

#define ITERATIONS 200

void
create_image(State* state) {
	Color pixels[windowWidth * windowHeight];

	for (int i = 0; i < windowHeight; i++) {
		for (int j = 0; j < windowWidth; j++) {

			Complex orig =
			    xy_to_complex(j, i, state->origin, state->zoom);
			Complex z = {orig.real, orig.imag};

			int count = 0;
			while (is_inside(z) && count < ITERATIONS) {
				count++;
				z = complex_add(complex_square(z), orig);
			}

			if (count >= ITERATIONS) {
				pixels[i * windowWidth + j] = BLACK;
			} else {
				pixels[i * windowWidth + j] = get_color(count);
			}
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
	state->texture = texture;
}

void
handle_input(State* state) {
	int key;
	do {
		key = GetKeyPressed();
		switch (key) {
		case (KEY_W):
			state->origin.imag -= 0.08 * (4 + state->zoom);
			break;
		case (KEY_S):
			state->origin.imag += 0.08 * (4 + state->zoom);
			break;
		case (KEY_A):
			state->origin.real -= 0.08 * (4 + state->zoom);
			break;
		case (KEY_D):
			state->origin.real += 0.08 * (4 + state->zoom);
			break;
		case (KEY_E):
			state->zoom -= 0.08;
			if (state->zoom <= -4) { state->zoom = -3.96; }
			break;
		case (KEY_Q): state->zoom += 0.08; break;
		}
	} while (key != 0);
}

int
main(void) {
	InitWindow(windowWidth, windowHeight, "Mandlebrot Set");
	SetWindowMonitor(0);

	State state = {
	    .texture = {0},
              .origin = {0, 0},
              .zoom = 0.0
        };

	Complex previous_origin = {0, 0};
	double previous_zoom = 1.0;

	while (!WindowShouldClose() && !quitGame) {
		BeginDrawing();
		ClearBackground(BLACK);

		if (complex_equal(state.origin, previous_origin) &&
		    state.zoom == previous_zoom) {
			DrawTexture(state.texture, 0, 0, WHITE);
		} else {
			UnloadTexture(state.texture);
			create_image(&state);
			previous_origin = state.origin;
			previous_zoom = state.zoom;
			printf("Zoom: %f\n", state.zoom);
		}

		DrawFPS(5, 5);
		EndDrawing();

		handle_input(&state);
	}

	CloseWindow();

	return 0;
}
