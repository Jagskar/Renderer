#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#define FPS 30
// Number of milliseconds in each frame = number of milliseconds in a second / number of frames per second
#define FRAME_TARGET_TIME (1000/FPS)

extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern uint32_t* colour_buffer;
extern SDL_Texture* colour_buffer_texture;
extern int window_width;
extern int window_height;

class Display
{
public:
	// Constructor
	Display();

	// Methods
	bool initialise_window();
	void draw_grid(uint32_t colour);
	void draw_dot_grid(uint32_t colour);
	void draw_pixel(int x, int y, uint32_t colour);
	void draw_rect(int x, int y, int width, int height, uint32_t colour);
	void render_colour_buffer();
	void clear_colour_buffer(uint32_t colour);
	void destroy_window();
};

#endif