#ifndef CRT_WINDOW_H
#define CRT_WINDOW_H

/// include the sdl graphics library. statically link for realease version.
#include "SDL.h"

// window dimension constants
const int WINDOW_WIDTH = 1920;  
const int WINDOW_HEIGHT = 1080;

// initialize the sdl resources that will be available to everyone
SDL_Renderer* renderer;
SDL_Window*   window;

// this function creates the window
// consider making this return a bool so that problems initializing the window can be detected
void initializeWindow() {
	/// initialize things
	SDL_Init(SDL_INIT_VIDEO);
	/// create the window and renderer
	window = SDL_CreateWindow("project two", 100, 200, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	/// set the render clear color, we set it to black so we can draw the letterbox
	//SDL_SetRenderDrawColor(renderer, 1, 1, 1, 1);
}

#endif