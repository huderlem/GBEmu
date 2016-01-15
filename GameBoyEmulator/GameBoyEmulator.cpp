// GameBoyEmulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define EXPORT_TEST_APIS 1

#include "GameBoy.h"

int main()
{
	GameBoy *gameboy = new GameBoy();
	gameboy->LoadGame("D:\\Downloads\\cpu_instrs\\cpu_instrs\\cpu_instrs.gb");
	gameboy->Run();
    return 0;
}

