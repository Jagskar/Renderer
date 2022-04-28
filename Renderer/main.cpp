#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"

Display display{};
bool is_running = false;

void setup()
{
	// Allocate memory to colour_buffer: size = size of a uint32_t object x window_width x window_height (number of pixels on the screen)
	colour_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);
	if (!colour_buffer)
		fprintf(stderr, "Error allocating memory to colour_buffer\n");

	// Create SDL texture to display the colour_buffer pixel values
	colour_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
	if (!colour_buffer_texture)
		fprintf(stderr, "Error instantiating colour_buffer_texture.\n");
}

void process_input()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	// Set is_running to false if the x button on the window is clicked or if the user presses the Esc key
	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE)
			is_running = false;
		break;
	}
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 189, 53, 122, 255);
	SDL_RenderClear(renderer);

	// display.draw_grid(0xFFBD357A);
	// display.draw_dot_grid(0xFFBD357A);
	// display.draw_pixel(200, 200, 0xFFFFFFFF);
	display.draw_rect(200, 200, 500, 200, 0xFFBD357A);

	display.render_colour_buffer();
	// Set each pixel on the screen to the colour black
	display.clear_colour_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
	is_running = display.initialise_window();

	setup();

	while (is_running)
	{
		process_input();
		render();
	}

	display.destroy_window();

	return 0;
}