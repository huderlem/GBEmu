#include "stdafx.h"
#include "LCDDisplay.h"


LCDDisplay::LCDDisplay(VRAM *vram)
{
	LCDDisplay::vram = vram;

	CoincidenceInterrupt = 0;
	OAMInterrupt = 0;
	VBlankInterrupt = 0;
	HBlankInterrupt = 0;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("GBEmu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160*2, 144*2, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	texture = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TextureAccess::SDL_TEXTUREACCESS_STREAMING, DISPLAY_PIXELS_WIDTH, DISPLAY_PIXELS_HEIGHT);
	SDL_RenderClear(renderer);

	InitPalette();

	time = SDL_GetTicks();
	last = time;
}

LCDDisplay::~LCDDisplay()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int LCDDisplay::ReadSTAT()
{
	int coincidenceFlag = LY == LYC ? 1 : 0;
	return (CoincidenceInterrupt << 6) | (OAMInterrupt << 5) | (VBlankInterrupt << 4) | (HBlankInterrupt << 3) | (coincidenceFlag << 2) | mode;
}

void LCDDisplay::WriteSTAT(int value)
{
	CoincidenceInterrupt = (value & 0b01000000) > 0 ? 1 : 0;
	OAMInterrupt = (value & 0b00100000) > 0 ? 1 : 0;
	VBlankInterrupt = (value & 0b00010000) > 0 ? 1 : 0;
	HBlankInterrupt = (value & 0b00001000) > 0 ? 1 : 0;
}

void LCDDisplay::SetAllPixels()
{
	SetBackgroundPixels();
	SetWindowPixels();
}

void LCDDisplay::SetBackgroundPixels()
{
	int tileXOffset = (SCX / 8);
	int tileYOffset = (SCY / 8);
	int pixelColumnOffset = (SCX % 8);
	int pixelRowOffset = (SCY % 8);
	int bgMapOffset;
	if ((LCDC & 0b00001000) > 0)
	{
		bgMapOffset = 0x400;
	}
	else
	{
		bgMapOffset = 0;
	}

	// Fill in all tiles' pixels.
	int pixelIndex = 0;
	int tileYCounter = 0;
	for (int pixelY = 0; pixelY < DISPLAY_PIXELS_HEIGHT; pixelY++)
	{
		int tileXCounter = 0;
		for (int pixelX = 0; pixelX < DISPLAY_PIXELS_WIDTH; pixelX++)
		{
			int xOffset = (tileXCounter + tileXOffset) % 0x20;
			int yOffset = (tileYCounter + tileYOffset) % 0x20;
			int tileSlot = (yOffset * 0x20) + xOffset + bgMapOffset;

			int tileId = vram->bgMapRAM[tileSlot];
			if (tileId < 0x80 && (LCDC & 0b00010000) == 0)
			{
				tileId += 0x100;
			}

			int tilePixelIndex = (tileId * 64) + (pixelRowOffset * 8) + pixelColumnOffset;
			int pixel = vram->tilePixels[tilePixelIndex];

			backgroundPixels[pixelIndex] = pixel;
			pixelIndex++;

			pixelColumnOffset = (pixelColumnOffset + 1) % 8;
			if (pixelColumnOffset == 0)
			{
				tileXCounter++;
			}
		}

		pixelRowOffset = (pixelRowOffset + 1) % 8;
		if (pixelRowOffset == 0)
		{
			tileYCounter++;
		}
	}
}

void LCDDisplay::SetWindowPixels()
{
	int pixelColumnOffset = ((8 - (WX - 7)) % 8);
	int pixelRowOffset = ((8 - WY) % 8);
	int bgMapOffset;
	if ((LCDC & 0b01000000) > 0)
	{
		bgMapOffset = 0x400;
	}
	else
	{
		bgMapOffset = 0;
	}

	// Fill in all tiles' pixels.
	int pixelIndex = 0;
	for (int pixelY = 0; pixelY < DISPLAY_PIXELS_HEIGHT; pixelY++)
	{
		for (int pixelX = 0; pixelX < DISPLAY_PIXELS_WIDTH; pixelX++)
		{
			if (pixelY < WY || pixelX < (WX - 7))
			{
				windowPixels[pixelIndex] = -1;
			}
			else
			{
				int xOffset = (pixelX - (WX - 7)) / 8;
				int yOffset = (pixelY - WY) / 8;
				int tileSlot = (yOffset * 0x20) + xOffset + bgMapOffset;

				int tileId = vram->bgMapRAM[tileSlot];
				if (tileId < 0x80 && (LCDC & 0b00010000) == 0)
				{
					tileId += 0x100;
				}

				int tilePixelIndex = (tileId * 64) + (pixelRowOffset * 8) + pixelColumnOffset;
				int pixel = vram->tilePixels[tilePixelIndex];

				windowPixels[pixelIndex] = pixel;
			}

			pixelIndex++;
			pixelColumnOffset = (pixelColumnOffset + 1) % 8;
		}

		pixelRowOffset = (pixelRowOffset + 1) % 8;
	}
}

void LCDDisplay::SetOAMPixels(void *pixels)
{
	// TODO: support 8x16 mode
	// TODO: correct overlapping sprite priorities and conflicts

	for (int oamId = 0; oamId < 40; oamId++)
	{
		int oamOffset = oamId * 4;
		int yPos = vram->oamData[oamOffset] - 16;
		int xPos = vram->oamData[oamOffset + 1] - 8;
		int tileId = vram->oamData[oamOffset + 2];
		int attributes = vram->oamData[oamOffset + 3];
		bool behindBG = (attributes & 0b10000000) > 0;
		bool yFlip = (attributes & 0b01000000) > 0;
		bool xFlip = (attributes & 0b00100000) > 0;
		unsigned char *nonGBCPaletteMap = (attributes & 0b00010000) > 0 ? vram->OBP1Map : vram->OBP0Map;

		// Set the sprite's pixels.
		int correctedX;
		int correctedY;
		for (int y = 0; y < 8; y++)
		{
			for (int x = 0; x < 8; x++)
			{
				// Skip if pixel would be off-screen.
				if (yPos + y < 0 || xPos + x < 0 || yPos + y > DISPLAY_PIXELS_HEIGHT || xPos + x > DISPLAY_PIXELS_WIDTH)
				{
					continue;
				}

				// Respect axis mirroring attributes.
				correctedX = x;
				correctedY = y;
				if (xFlip)
				{
					correctedX = 7 - x;
				}
				if (yFlip)
				{
					correctedY = 7 - y;
				}

				int tilePixelOffset = (tileId * 64) + (correctedY * 8) + correctedX;
				int newPixel = vram->tilePixels[tilePixelOffset];

				// Color 0 is transparent for OAM sprites.
				if (newPixel > 0)
				{
					int pixelIndex = (yPos * DISPLAY_PIXELS_WIDTH) + xPos + (y * DISPLAY_PIXELS_WIDTH) + x;

					// Don't draw sprite pixel if it's behind the Background or Window colors 1-3.
					if (behindBG && 
						(((LCDC & 0b00000001) > 0 && backgroundPixels[pixelIndex] > 0)
						 || ((LCDC & 0b00100000) > 0 && windowPixels[pixelIndex] > 0)))
					{
						continue;
					}

					((Uint32 *)(pixels))[pixelIndex] = palette[nonGBCPaletteMap[newPixel]];
				}
			}
		}
	}
}

void LCDDisplay::InitPalette()
{
	palette = new Uint32[4]();
	palette[0] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0xFF, 0xFF, 0xFF);
	palette[1] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0xA0, 0xA0, 0xA0);
	palette[2] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0x60, 0x60, 0x60);
	palette[3] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0x00, 0x00, 0x00);
}

void LCDDisplay::SetDisplayPixels()
{
	// Lock texture for pixel manipulation.
	void *pixels;
	int pitch;
	SDL_LockTexture(texture, NULL, &pixels, &pitch);

	// Set new pixels on the current scan line for background and window.
	for (int i = 0; i < DISPLAY_PIXELS_HEIGHT * DISPLAY_PIXELS_WIDTH; i++)
	{
		int newPixel;
		if ((LCDC & 0b00100000) > 0 && windowPixels[i] != -1)
		{
			newPixel = windowPixels[i];
		}
		else if ((LCDC & 0b00000001) > 0)
		{
			newPixel = backgroundPixels[i];
		}
		else
		{
			newPixel = 0;
		}

		((Uint32 *)(pixels))[i] = palette[vram->BGPMap[newPixel]];
	}

	// Draw OAM sprites.
	if ((LCDC & 0b00000010) > 0)
	{
		SetOAMPixels(pixels);
	}

	// Unlock texture.
	SDL_UnlockTexture(texture);
}

void LCDDisplay::SetDisplayPixels(int *newPixels, int numPixels, int offset)
{
	// Lock texture for pixel manipulation.
	void *pixels;
	int pitch;
	SDL_LockTexture(texture, NULL, &pixels, &pitch);

	// Set new pixels for texture.
	for (int i = 0; i < numPixels; i++)
	{
		int newPixel = newPixels[i];
		((Uint32 *)(pixels))[i + offset] = palette[vram->BGPMap[newPixel]];
	}

	// Unlock texture.
	SDL_UnlockTexture(texture);
}

void LCDDisplay::Render()
{
	SetAllPixels();
	SetDisplayPixels();

	SDL_RenderClear(renderer);

	// Only render if the LCDC flag is set.
	if ((LCDC & 0b10000000) > 0)
	{
		SDL_RenderCopy(renderer, texture, NULL, NULL);
	}

	SDL_RenderPresent(renderer);
}

void LCDDisplay::Tick(int cpuCycles, Interrupts *interrupts)
{
	ticks += cpuCycles;
	if (mode == 0) // H-Blank
	{
		if (ticks >= 204)
		{
			// End H-Blank period
			ticks = ticks % 204;
			LY++;
			if (LY < 144)
			{
				mode = 2;

				if (OAMInterrupt > 0)
				{
					interrupts->RequestLCDStatInterrupt();
				}
			}
			else
			{
				// Enter V-Blank period
				mode = 1;

				// Render the current line
				Render();
				
				interrupts->RequestVBlankInterrupt();
				if (VBlankInterrupt > 0)
				{
					interrupts->RequestLCDStatInterrupt();
				}

				time = SDL_GetTicks();
				printf("Time: %f\n", 1000.0 / (time - last));
				last = time;
			}

			if (CoincidenceInterrupt > 0 && LY == LYC)
			{
				interrupts->RequestLCDStatInterrupt();
			}
		}
	}
	else if (mode == 1) // V-Blank
	{
		int prevLY = LY;
		LY = 144 + (ticks / 456);
		if (ticks >= 4560)
		{
			// End V-Blank period
			ticks = ticks % 4560;
			LY = 0;
			mode = 2;

			if (OAMInterrupt > 0)
			{
				interrupts->RequestLCDStatInterrupt();
			}
			if (CoincidenceInterrupt > 0 && LY == LYC)
			{
				interrupts->RequestLCDStatInterrupt();
			}
		}
		else if (LY != prevLY)
		{
			if (CoincidenceInterrupt > 0 && LY == LYC)
			{
				interrupts->RequestLCDStatInterrupt();
			}
		}
	}
	else if (mode == 2) // LCD controller is Reading OAM RAM
	{
		if (ticks >= 80)
		{
			// End Reading OAM RAM period
			ticks = ticks % 80;
			mode = 3;
		}
	}
	else if (mode == 3) // Transferring Data to LCD Driver
	{
		if (ticks >= 172)
		{
			// End Transfer Data period
			ticks = ticks % 172;
			mode = 0;

			if (HBlankInterrupt > 0)
			{
				interrupts->RequestLCDStatInterrupt();
			}
		}
	}
}
