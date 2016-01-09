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
	while (running)
	{
		int cpuCycles = cpu->ExecuteNextInstruction();
	}
}
