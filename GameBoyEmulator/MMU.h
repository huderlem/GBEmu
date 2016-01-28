#pragma once

#include "BaseMBC.h"
#include "IMMU.h"
#include "Interrupts.h"
#include "Joypad.h"
#include "LCDDisplay.h"
#include "MBC1.h"
#include "MBC2.h"
#include "MBC3.h"
#include "MBC5.h"
#include "Timer.h"
#include "VRAM.h"
#include "WRAM.h"

class MMU: public IMMU
{
public:
	MMU(Interrupts *interrupts, WRAM *wram, VRAM *vram, Joypad *joypad, Timer *timer, LCDDisplay *display);
	~MMU();
	virtual int ReadByte(long address);
	virtual int ReadWord(long address);
	virtual void WriteByte(int value, long address);
	virtual void WriteWord(int value, long address);
	virtual bool LoadROM(std::string filepath);
	virtual void InitializeMBC(std::string romDirectory, std::string romName);
	virtual void ExitGame();
	virtual void TickMBC(int cpuCycles, int cyclesPerSecond);
private:
	// Byte array of entire ROM.
	unsigned char *ROM;
	long ROMSize;

	// Working RAM
	WRAM *wram;

	// Video RAM
	VRAM *vram;

	// LCD Display
	LCDDisplay *display;

	// High RAM
	unsigned char *HRAM;
	long HRAMSize;
	void InitializeHRAM();

	void DMATransfer(int value);

	// Memory Bank Controller
	BaseMBC *mbc;

	Joypad *joypad;

	Interrupts *interrupts;

	Timer *timer;

	// ROM headers
	int cartType;
	int ROMSizeType;
	int RAMSizeType;
	int destinationCode;
};
