#include "Display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* colour_buffer = NULL;
SDL_Texture* colour_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

Display::Display()
{
}

bool Display::initialise_window()
{
	// Check if errors exist when initialising SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		fprintf(stderr, "Error initialising SDL.\n");
		return false;
	}

	// Get max width and height of current display
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);
	// Set window dimensions
	window_width = display_mode.w;
	window_height = display_mode.h;

	// Create SDL window, center in x and y axis
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);

	// Check for errors in initialisation
	if (!window)
	{
		fprintf(stderr, "Error creating SDL window.\n");
		return false;
	}

	// Create SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	// Check for errors in initialisation
	if (!renderer)
	{
		fprintf(stderr, "Error creating SDL renderer.\n");
		return false;
	}

	return true;
}

void Display::draw_grid(uint32_t colour)
{
	for (int y = 0; y < window_height; y++)
		for (int x = 0; x < window_width; x++)
			if (x % 10 == 0 || y % 10 == 0)
				colour_buffer[(window_width * y) + x] = colour;
}

void Display::draw_dot_grid(uint32_t colour)
{
	for (int y = 0; y < window_height; y += 10)
		for (int x = 0; x < window_width; x += 10)
			colour_buffer[(window_width * y) + x] = colour;
}

void Display::draw_pixel(int x, int y, uint32_t colour)
{
	if (x < window_width && y < window_height)
		colour_buffer[(window_width * y) + x] = colour;
}

void Display::draw_rect(int x, int y, int width, int height, uint32_t colour)
{
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			int current_x = x + i;
			int current_y = y + j;

			colour_buffer[(window_width * current_y) + current_x] = colour;
		}
}

void Display::render_colour_buffer()
{
	// Copy colour_buffer pixel values to colour_buffer_texture
	// set pitch = the number of pixels in a row on the screen = window_width * size of each pixel
	SDL_UpdateTexture(colour_buffer_texture, NULL, colour_buffer, (int)(window_width * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, colour_buffer_texture, NULL, NULL);
}

void Display::clear_colour_buffer(uint32_t colour)
{
	// Set the pixel value of each pixel on the screen
	for (int y = 0; y < window_height; y++)
		for (int x = 0; x < window_width; x++)
			colour_buffer[(window_width * y) + x] = colour;
}

void Display::destroy_window()
{
	// Release memory allocated to colour_buffer
	free(colour_buffer);

	// Undo operations in initialise_window in reverse order
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}