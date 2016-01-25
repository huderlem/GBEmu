#pragma once
#include "SDL.h"

#include "CPU.h"
#include "Interrupts.h"

class Joypad
{
public:
	Joypad();
	~Joypad();
	int ReadJoypad();
	void WriteJoyPad(int value);
	bool ProcessJoypadInput(Interrupts *interrupts, CPU *cpu);
private:
	// I/O port 0xFF00
	int JOYP;
	bool ButtonsSelected = false;
	bool DirectionsSelected = false;

	// Joypad states. 0=Pressed, 1=Not Pressed
	char A;
	char B;
	char Start;
	char Select;
	char Right;
	char Left;
	char Up;
	char Down;

	bool ProcessKeyInput(char *key, Uint8 keyState);
};

