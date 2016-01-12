// GameBoyEmulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#define EXPORT_TEST_APIS 1

#include "GameBoy.h"

int main()
{
	GameBoy *gameboy = new GameBoy();
	gameboy->LoadGame("D:\\Games\\Emulation\\Nintendo Gameboy\\Tetris\\Tetris.gb");
	gameboy->Run();
    return 0;
}

