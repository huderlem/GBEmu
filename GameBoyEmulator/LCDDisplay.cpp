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

	// Lock texture for pixel manipulation.
	SDL_LockTexture(texture, NULL, &pixels, &pitch);

	LCDC = 0;

	InitPalette();

	time = SDL_GetTicks();
	lastRenderTime = time;
	ticksPerFrame = 1000.0 / 60.0;

	oamIds = new int[40]();
	for (int i = 0; i < 40; i++)
	{
		oamIds[i] = i;
	}
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

void LCDDisplay::SetScanlinePixels(int scanline)
{
	SetBackgroundPixels(scanline);
	SetWindowPixels(scanline);
}

void LCDDisplay::SetBackgroundPixels(int scanline)
{
	int tileXOffset = (SCX / 8);
	int pixelColumnOffset = (SCX % 8);
	int pixelRowOffset = ((SCY + scanline) % 8) * 8;
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
	int pixelIndex = scanline * DISPLAY_PIXELS_WIDTH;
	int yOffset = (((SCY + scanline) / 8) % 0x20) * 0x20;
	int tileXCounter = 0;
	bool secondTileLocation = (LCDC & 0b00010000) == 0;
	for (int pixelX = 0; pixelX < DISPLAY_PIXELS_WIDTH; pixelX++)
	{
		int xOffset = (tileXCounter + tileXOffset) % 0x20;
		int tileSlot = yOffset + xOffset + bgMapOffset;

		int tileId = vram->bgMapRAM[tileSlot];
		if (tileId < 0x80 && secondTileLocation)
		{
			tileId += 0x100;
		}

		int tilePixelIndex = (tileId * 64) + pixelRowOffset + pixelColumnOffset;
		int pixel = vram->tilePixels[tilePixelIndex];

		backgroundPixels[pixelIndex] = pixel;
		pixelIndex++;

		pixelColumnOffset = (pixelColumnOffset + 1) % 8;
		if (pixelColumnOffset == 0)
		{
			tileXCounter++;
		}
	}
}

void LCDDisplay::SetWindowPixels(int scanline)
{
	int WXAdjusted = WX - 7;
	int pixelColumnOffset = ((8 - WXAdjusted) % 8);
	int pixelRowOffset = ((((8 - WY) % 8) + scanline) % 8) * 8;
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
	bool secondTileLocation = (LCDC & 0b00010000) == 0;
	int pixelIndex = scanline * DISPLAY_PIXELS_WIDTH;
	int pixelY = scanline;
	int yOffset = ((scanline - WY) / 8) * 0x20;
	for (int pixelX = 0; pixelX < DISPLAY_PIXELS_WIDTH; pixelX++)
	{
		if (pixelY < WY || pixelX < WXAdjusted)
		{
			windowPixels[pixelIndex] = -1;
		}
		else
		{
			int xOffset = (pixelX - WXAdjusted) / 8;
			int tileSlot = yOffset + xOffset + bgMapOffset;

			int tileId = vram->bgMapRAM[tileSlot];
			if (tileId < 0x80 && secondTileLocation)
			{
				tileId += 0x100;
			}

			int tilePixelIndex = (tileId * 64) + pixelRowOffset + pixelColumnOffset;
			int pixel = vram->tilePixels[tilePixelIndex];

			windowPixels[pixelIndex] = pixel;
		}

		pixelIndex++;
		pixelColumnOffset = (pixelColumnOffset + 1) % 8;
	}
}

void LCDDisplay::SetOAMPixels(void *pixels, int scanline)
{
	int usedSpriteIds = 0;
	//SortOAMIds(oamIds, 0, 39);
	int pixelsTaken[160] = {};
	for (int i = 0; i < 40; i++)
	{
		// Max of 10 sprites per scanline.
		if (usedSpriteIds >= 10)
		{
			break;
		}

		int oamId = oamIds[i];

		bool spriteInScanline = false;
		int oamOffset = oamId * 4;
		int yPos = vram->oamData[oamOffset] - 16;
		int xPos = vram->oamData[oamOffset + 1] - 8;
		int tileId = vram->oamData[oamOffset + 2];
		int attributes = vram->oamData[oamOffset + 3];
		bool behindBG = (attributes & 0b10000000) > 0;
		bool yFlip = (attributes & 0b01000000) > 0;
		bool xFlip = (attributes & 0b00100000) > 0;
		unsigned char *nonGBCPaletteMap = (attributes & 0b00010000) > 0 ? vram->OBP1Map : vram->OBP0Map;

		// Respect 8x16 mode.
		int spriteHeight;
		if ((LCDC & 0b00000100) > 0)
		{
			spriteHeight = 16;
			tileId &= 0b11111110;
		}
		else
		{
			spriteHeight = 8;
		}

		if (yPos > scanline || yPos + spriteHeight <= scanline)
		{
			// Sprite isn't on current scanline.
			continue;
		}

		int y = scanline - yPos;

		// Set the sprite's pixels.
		int correctedX;
		int correctedY;
		for (int x = 0; x < 8; x++)
		{
			int pixelColumn = xPos + x;

			// Skip if pixel would be off-screen.
			if (pixelColumn < 0 || pixelColumn >= DISPLAY_PIXELS_WIDTH || pixelsTaken[pixelColumn] > 0)
			{
				continue;
			}

			spriteInScanline = true;

			// Respect axis mirroring attributes.
			correctedX = x;
			correctedY = y;
			if (xFlip)
			{
				correctedX = 7 - x;
			}
			if (yFlip)
			{
				correctedY = spriteHeight - 1 - y;
			}

			int tilePixelOffset = (tileId * 64) + (correctedY * 8) + correctedX;
			int newPixel = vram->tilePixels[tilePixelOffset];

			// Color 0 is transparent for OAM sprites.
			if (newPixel > 0)
			{
				int pixelIndex = (scanline * DISPLAY_PIXELS_WIDTH) + pixelColumn;

				// Don't draw sprite pixel if it's behind the Background or Window colors 1-3.
				if (behindBG &&
					(((LCDC & 0b00000001) > 0 && backgroundPixels[pixelIndex] > 0)
						|| ((LCDC & 0b00100000) > 0 && windowPixels[pixelIndex] > 0)))
				{
					continue;
				}

				((Uint32 *)(pixels))[pixelIndex] = palette[nonGBCPaletteMap[newPixel]];
				pixelsTaken[pixelColumn] = 1;
			}
		}

		if (spriteInScanline)
		{
			usedSpriteIds++;
		}
	}
}

// Perorm a merge sort to arrange the sprite data in correct priority order.
void LCDDisplay::SortOAMIds(int * oamIds, int lower, int upper)
{
	if (upper <= lower)
	{
		return;
	}

	int middle = (lower + upper) / 2;
	SortOAMIds(oamIds, lower, middle);
	SortOAMIds(oamIds, middle + 1, upper);

	SortOAMIds_Merge(oamIds, lower, upper, middle);
}

void LCDDisplay::SortOAMIds_Merge(int * oamIds, int lower, int upper, int middle)
{
	int leftSize = middle - lower + 1;
	int rightSize = upper - middle;
	int * left = new int[leftSize];
	int * right = new int[rightSize];

	for (int i = 0; i < leftSize; i++)
	{
		left[i] = oamIds[lower + i];
	}

	for (int i = 0; i < rightSize; i++)
	{
		right[i] = oamIds[middle + 1 + i];
	}

	int leftIndex = 0;
	int rightIndex = 0;
	int resultIndex = lower;
	while (leftIndex < leftSize && rightIndex < rightSize)
	{
		int leftOAMId = left[leftIndex];
		int rightOAMId = right[rightIndex];
		int leftXPos = vram->oamData[(leftOAMId * 4) + 1] - 8;
		int rightXPos = vram->oamData[(rightOAMId * 4) + 1] - 8;
		bool pushLeft = true;
		if (leftXPos == rightXPos)
		{
			pushLeft = leftOAMId < rightOAMId;
		}
		else
		{
			pushLeft = leftXPos < rightXPos;
		}

		if (pushLeft)
		{
			oamIds[resultIndex] = left[leftIndex];
			leftIndex++;
		}
		else
		{
			oamIds[resultIndex] = right[rightIndex];
			rightIndex++;
		}

		resultIndex++;
	}

	while (leftIndex < leftSize)
	{
		oamIds[resultIndex] = left[leftIndex];
		leftIndex++;
		resultIndex++;
	}

	while (rightIndex < rightSize)
	{
		oamIds[resultIndex] = right[rightIndex];
		rightIndex++;
		resultIndex++;
	}

	delete [] left;
	delete [] right;
}

void LCDDisplay::InitPalette()
{
	palette = new Uint32[4]();
	palette[0] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0xFF, 0xFF, 0xFF);
	palette[1] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0xA0, 0xA0, 0xA0);
	palette[2] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0x60, 0x60, 0x60);
	palette[3] = SDL_MapRGB(SDL_GetWindowSurface(window)->format, 0x00, 0x00, 0x00);
}

void LCDDisplay::SetDisplayPixels(int scanline, void *pixels, int pitch)
{
	// Set new pixels on the current scan line for background and window.
	int startIndex = scanline * DISPLAY_PIXELS_WIDTH;
	for (int i = startIndex; i < startIndex + DISPLAY_PIXELS_WIDTH; i++)
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
		SetOAMPixels(pixels, scanline);
	}
}

void LCDDisplay::Render()
{
	// Unlock texture.
	SDL_UnlockTexture(texture);

	SDL_RenderClear(renderer);

	// Only render if the LCDC flag is set.
	if ((LCDC & 0b10000000) > 0)
	{
		SDL_RenderCopy(renderer, texture, NULL, NULL);
	}

	SDL_RenderPresent(renderer);

	Uint32 timePassed = SDL_GetTicks() - lastRenderTime;
	long long timeToDelay = ticksPerFrame - timePassed;
	if (timeToDelay > 0)
	{
		//SDL_Delay(timeToDelay);
	}

	lastRenderTime = SDL_GetTicks();

	SDL_LockTexture(texture, NULL, &pixels, &pitch);
}

void LCDDisplay::Tick(int cpuCycles, Interrupts *interrupts, CPU *cpu)
{
	// LCD is turned off during STOP mode.
	if (cpu->InStopMode() || (LCDC & 0x80) == 0)
	{
		return;
	}

	ticks += cpuCycles;
	if (mode == 0) // H-Blank
	{
		if (ticks >= 204)
		{
			// Render the current line
			SetScanlinePixels(LY);
			SetDisplayPixels(LY, pixels, pitch);

			// End H-Blank period
			ticks = ticks % 204;
			LY++;
			if (LY < 144)
			{
				mode = 2;

				if (OAMInterrupt > 0)
				{
					interrupts->RequestLCDStatInterrupt();
					cpu->NotifyInterruptOccurred();
				}
			}
			else
			{
				// Enter V-Blank period
				mode = 1;
				
				interrupts->RequestVBlankInterrupt();
				cpu->NotifyInterruptOccurred();
				if (VBlankInterrupt > 0)
				{
					interrupts->RequestLCDStatInterrupt();
					cpu->NotifyInterruptOccurred();
				}
			}

			if (CoincidenceInterrupt > 0 && LY == LYC)
			{
				interrupts->RequestLCDStatInterrupt();
				cpu->NotifyInterruptOccurred();
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
			Render();

			ticks = ticks % 4560;
			LY = 0;
			mode = 2;

			if (OAMInterrupt > 0)
			{
				interrupts->RequestLCDStatInterrupt();
				cpu->NotifyInterruptOccurred();
			}
			if (CoincidenceInterrupt > 0 && LY == LYC)
			{
				interrupts->RequestLCDStatInterrupt();
				cpu->NotifyInterruptOccurred();
			}
		}
		else if (LY != prevLY)
		{
			if (CoincidenceInterrupt > 0 && LY == LYC)
			{
				interrupts->RequestLCDStatInterrupt();
				cpu->NotifyInterruptOccurred();
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
				cpu->NotifyInterruptOccurred();
			}
		}
	}
}
