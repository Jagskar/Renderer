#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>
#include "Display.h"
#include "Vector.h"

Display display{};
bool is_running = false;
uint32_t grid_colour = 0xFFBD357A;

// Number of points in cube 9 x 9 x 9
const int n_points = 9 * 9 * 9;
vec3_t cube_points[n_points];
vec2_t projected_points[n_points];
vec3_t camera_position = { 0, 0, -5 };
// Angle rotation about the x, y and z axes
vec3_t cube_rotation = { 0, 0, 0 };

const float fov_factor_orthographic = 128;
const float fov_factor_perspective = 640;

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

	int point_count = 0;
	// Fill array of vectors to represent points of the (9x9x9) cube
	// Values range from -1 to 1
	for (float x = -1; x <= 1; x += 0.25)
		for (float y = -1; y <= 1; y += 0.25)
			for (float z = -1; z <= 1; z += 0.25)
			{
				vec3_t new_point = { x, y, z };
				cube_points[point_count] = new_point;
				point_count++;
			}
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

// Project the 3D point to 2D using orthographic projection
vec2_t orthographic_project(vec3_t point)
{
	vec2_t projected_point = {
		(fov_factor_orthographic * point.x),
		(fov_factor_orthographic * point.y)
	};

	return projected_point;
}

// Project the 3D point to 2D using perspective projection
vec2_t perspective_project(vec3_t point)
{
	vec2_t projected_point = {
		(fov_factor_perspective * point.x) / point.z,
		(fov_factor_perspective * point.y) / point.z
	};

	return projected_point;
}

void update()
{
	// Rotate about the x axis by 0.01 each frame
	cube_rotation.x += 0.01;
	// Rotate about the y axis by 0.01 each frame
	cube_rotation.y += 0.01;
	// Rotate about the z axis by 0.01 each frame
	cube_rotation.z += 0.01;

	for (int i = 0; i < n_points; i++)
	{
		vec3_t point = cube_points[i];

		vec3_t transformed_point = vec3_rotate_x(point, cube_rotation.x);
		transformed_point = vec3_rotate_y(transformed_point, cube_rotation.y);
		transformed_point = vec3_rotate_z(transformed_point, cube_rotation.z);

		// Move points away from camera
		transformed_point.z -= camera_position.z;

		//---------- Project point ----------
		// Orthographic
		// vect2_t projected_point = orthographic_project(point);
		//
		// Perspective
		vec2_t projected_point = perspective_project(transformed_point);
		//------------------------------------

		projected_points[i] = projected_point;
	}
}

void render()
{
	SDL_SetRenderDrawColor(renderer, 189, 53, 122, 255);
	SDL_RenderClear(renderer);

	// display.draw_grid(0xFFBD357A);
	// display.draw_dot_grid(0xFFBD357A);
	// display.draw_pixel(200, 200, 0xFFFFFFFF);

	for (int i = 0; i < n_points; i++)
	{
		vec2_t projected_point = projected_points[i];
		display.draw_rect(projected_point.x + window_width / 2, projected_point.y + window_height / 2, 4, 4, 0xFFFFFFFF);
	}

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
		update();
		render();
	}

	display.destroy_window();

	return 0;
}