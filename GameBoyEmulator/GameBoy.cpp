#include "stdafx.h"
#include "GameBoy.h"


GameBoy::GameBoy()
{
	interrupts = new Interrupts();
	wram = new WRAM();
	vram = new VRAM();
	timer = new Timer();
	mmu = new MMU(interrupts, wram, vram, joypad, timer);
	registers = new CPURegisters();
	opcodes = new CPUOpcodes(mmu, interrupts);
	cpu = new CPU(mmu, registers, opcodes);
	display = new LCDDisplay(vram);
}

GameBoy::~GameBoy()
{
	// TODO: delete members?
}

void GameBoy::LoadGame(std::string filepath)
{
	mmu->LoadROM(filepath);
}

void GameBoy::Run()
{
	running = true;
	SDL_Event event;
	int ticks = SDL_GetTicks();
	int num = 0;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			// Later, you'll be adding your code that handles keyboard / mouse input here
		}

		//int cpuCycles = cpu->ExecuteNextInstruction();
		display->Render();
		int now = SDL_GetTicks();
		if (num % 100 == 0)
		{
			printf("Time: %f\n", 1000.0 / (now - ticks));
		}
		num++;
		ticks = now;
	}
}
