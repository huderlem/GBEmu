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
	void LoadGame(std::string romDirectory, std::string romFilepath);
	void Run();
	void ExitGame();
private:
	void ReadCartHeader();
	void PowerUpSequence();

	std::string romDirectory;
	std::string romFilename;

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
	SoundController *soundController;
	IMMU *mmu;
	CPURegisters *registers;
	CPU *cpu;
	LCDDisplay *display;

	bool running = false;
};
