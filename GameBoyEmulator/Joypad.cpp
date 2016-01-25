#include "stdafx.h"

#include "Joypad.h"


Joypad::Joypad()
{
	A = 1;
	B = 1;
	Start = 1;
	Select = 1;
	Right = 1;
	Left = 1;
	Up = 1;
	Down = 1;
}

Joypad::~Joypad()
{
}

int Joypad::ReadJoypad()
{
	int state = 0;
	if (ButtonsSelected)
	{
		state |= 0b00010000 | ((Start << 3) | (Select << 2) | (B << 1) | A);
	}

	if (DirectionsSelected)
	{
		state |= 0b00100000 | ((Down << 3) | (Up << 2) | (Left << 1) | Right);
	}

	state |= 0b11000000;

	return state;
}

void Joypad::WriteJoyPad(int value)
{
	ButtonsSelected = ((value & 0b00100000) == 0);
	DirectionsSelected = ((value & 0b00010000) == 0);
}

bool Joypad::ProcessJoypadInput(Interrupts *interrupts, CPU *cpu)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_PumpEvents();

	bool newButtonPressed = false;
	newButtonPressed |= ProcessKeyInput(&A, keys[SDL_SCANCODE_A]);
	newButtonPressed |= ProcessKeyInput(&B, keys[SDL_SCANCODE_S]);
	newButtonPressed |= ProcessKeyInput(&Start, keys[SDL_SCANCODE_RETURN]);
	newButtonPressed |= ProcessKeyInput(&Select, keys[SDL_SCANCODE_BACKSPACE]);
	newButtonPressed |= ProcessKeyInput(&Right, keys[SDL_SCANCODE_RIGHT]);
	newButtonPressed |= ProcessKeyInput(&Left, keys[SDL_SCANCODE_LEFT]);
	newButtonPressed |= ProcessKeyInput(&Up, keys[SDL_SCANCODE_UP]);
	newButtonPressed |= ProcessKeyInput(&Down, keys[SDL_SCANCODE_DOWN]);

	if (newButtonPressed)
	{
		interrupts->RequestJoypadInterrupt();
		cpu->NotifyInterruptOccurred();
	}

	if (keys[SDL_SCANCODE_ESCAPE])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Joypad::ProcessKeyInput(char *key, Uint8 keyState)
{
	int previousKeyState = *key;
	*key = 1 - keyState;
	if (previousKeyState == 1 && *key == 0)
	{
		return true;
	}

	return false;
}
