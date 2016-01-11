#pragma once

#include "SDL.h"
#undef main

#include "VRAM.h"


class LCDDisplay
{
public:
	LCDDisplay(VRAM *vram);
	~LCDDisplay();
	void SetAllPixels();
	void SetDisplayPixels(int *newPixels, int numPixels, int offset);
	void Render();
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *screen;
	SDL_Texture *texture;

	int LCDC = 0;
	int STAT = 0;
	int SCY = 0;
	int SCX = 0;
	int LY = 0;
	int LYC = 0;
	int WY = 0;
	int WX = 0;
	int BGP = 0;
	int OBP0 = 0;
	int OBP1 = 0;

	// Constants
	const int DISPLAY_PIXELS_WIDTH = 160;
	const int DISPLAY_PIXELS_HEIGHT = 144;

	// Pixel buffers.
	int *backgroundPixels = new int[DISPLAY_PIXELS_WIDTH * DISPLAY_PIXELS_HEIGHT]();
	int *windowPixels = new int[DISPLAY_PIXELS_WIDTH * DISPLAY_PIXELS_HEIGHT]();

	// Palettes
	Uint32 *palette;
	void InitPalette();

	VRAM *vram;

	void SetDisplayPixels();
	void SetWindowPixels();
	void SetBackgroundPixels();
	void SetOAMPixels(void *pixels);
};
