#pragma once

#include <string>

#include "CPU.h"
#include "Interrupts.h"
#include "Joypad.h"
#include "LCDDisplay.h"
#include "MMU.h"
#include "Timer.h"
#include "VRAM.h"
#include "WRAM.h"

class GameBoy
{
public:
	GameBoy();
	~GameBoy();
	void LoadGame(std::string filepath);
	void Run();
private:
	void ReadCartHeader();
	void PowerUpSequence();

	// Cartridge Header Values
	std::string title = "";
	int cartType;
	int ROMSize;
	int RAMSize;
	int destinationCode;

	Interrupts *interrupts;
	WRAM *wram;
	VRAM *vram;
	Joypad *joypad;
	Timer *timer;
	IMMU *mmu;
	CPURegisters *registers;
	CPUOpcodes *opcodes;
	CPU *cpu;
	LCDDisplay *display;

	bool running = false;
};
