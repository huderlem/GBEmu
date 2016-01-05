#pragma once

#include "BaseMBC.h"
#include "IMMU.h"
#include "Interrupts.h"
#include "Joypad.h"
#include "Timer.h"
#include "WRAM.h"

class MMU: public IMMU
{
public:
	MMU(Interrupts *interrupts, WRAM *wram, Joypad *joypad, Timer *timer);
	~MMU();
	virtual int ReadByte(long address);
	virtual int ReadWord(long address);
	virtual void WriteByte(int value, long address);
	virtual void WriteWord(int value, long address);
	virtual bool LoadROM(std::string filepath);
private:
	// Byte array of entire ROM.
	unsigned char *ROM;
	long ROMSize;

	// Working RAM
	WRAM *wram;

	// High RAM
	unsigned char *HRAM;
	long HRAMSize;
	void InitializeHRAM();

	// Memory Bank Controller
	BaseMBC *mbc;

	Joypad *joypad;

	Interrupts *interrupts;

	Timer *timer;
};
