#include "stdafx.h"
#include "GameBoy.h"


GameBoy::GameBoy()
{
	interrupts = new Interrupts();
	wram = new WRAM();
	vram = new VRAM();
	display = new LCDDisplay(vram);
	joypad = new Joypad();
	timer = new Timer();
	mmu = new MMU(interrupts, wram, vram, joypad, timer, display);
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
	ReadCartHeader();
}

void GameBoy::ReadCartHeader()
{
	for (int i = 0x134; i < 0x144; i++)
	{
		title.append(1, mmu->ReadByte(i));
	}

	cartType = mmu->ReadByte(0x147);
	ROMSize = mmu->ReadByte(0x148);
	RAMSize = mmu->ReadByte(0x149);
	destinationCode = mmu->ReadByte(0x14A);
}

void GameBoy::PowerUpSequence()
{
	// TODO: this is different for different systems.
	registers->a = 0x01;
	registers->f = 0xB0;
	registers->b = 0x00;
	registers->c = 0x13;
	registers->d = 0x00;
	registers->e = 0xD8;
	registers->h = 0x01;
	registers->l = 0x4D;
	registers->sp = 0xFFFE;
	registers->pc = 0x100;

	timer->WriteByte(0x00, 0xFF05);
	timer->WriteByte(0x00, 0xFF06);
	timer->WriteByte(0x00, 0xFF07);

	display->LCDC = 0x91;
	display->SCY = 0x00;
	display->SCX = 0x00;
	display->LYC = 0x00;
	vram->WriteBGP(0xFC);
	vram->WriteOBP0(0xFF);
	vram->WriteOBP1(0xFF);
	display->WY = 0x00;
	display->WX = 0x00;

	interrupts->SetInterruptEnableRegister(0x00);
}

void GameBoy::Run()
{
	running = true;

	// Entry point for game
	PowerUpSequence();

	while (running)
	{
		joypad->ProcessJoypadInput(interrupts, cpu);

		int cpuCycles = cpu->ExecuteNextInstruction(interrupts);
		display->Tick(cpuCycles, interrupts, cpu);
		timer->Tick(cpuCycles, interrupts, cpu);
		
		interrupts->ExecutePendingInterrupt(registers, mmu);
	}
}

