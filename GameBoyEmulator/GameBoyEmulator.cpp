// GameBoyEmulator.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <string>

#include "GameBoy.h"

int main()
{
	std::string directory = "D:\\Games\\Emulation\\Nintendo Gameboy\\Pokemon Gold\\";
	std::string romFilename = "Pokemon Gold.gbc";

	GameBoy *gameboy = new GameBoy();
	gameboy->LoadGame(directory, romFilename);
	gameboy->Run();
    return 0;
}

