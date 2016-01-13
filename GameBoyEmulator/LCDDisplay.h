#pragma once

#include "SDL.h"
#undef main

#include "Interrupts.h"
#include "VRAM.h"


class LCDDisplay
{
public:
	LCDDisplay(VRAM *vram);
	~LCDDisplay();
	int ReadSTAT();
	void WriteSTAT(int value);
	void SetAllPixels();
	void SetDisplayPixels(int *newPixels, int numPixels, int offset);
	void Render();
	void Tick(int cpuCycles, Interrupts *interrupts);

	int LCDC = 0;

	// STAT register
	int CoincidenceInterrupt;
	int OAMInterrupt;
	int VBlankInterrupt;
	int HBlankInterrupt;

	int SCY = 0;
	int SCX = 0;
	int LY = 0;
	int LYC = 0;
	int WY = 0;
	int WX = 0;
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Surface *screen;
	SDL_Texture *texture;

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

	int ticks = 0;
	int mode = 0;  // See STAT register

	int time = 0;
	int last = 0;
};
