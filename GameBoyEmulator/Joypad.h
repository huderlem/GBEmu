#pragma once
#include "SDL.h"

#include "Interrupts.h"

class Joypad
{
public:
	Joypad();
	~Joypad();
	char ReadJoypad();
	void WriteJoyPad(int value);
	void ProcessJoypadInput(Interrupts *interrupts);
private:
	// I/O port 0xFF00
	char JOYP;
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

