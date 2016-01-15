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

char Joypad::ReadJoypad()
{
	int state = 0;
	if (ButtonsSelected)
	{
		state = ((Start << 3) | (Select << 2) | (B << 1) | A);
	}
	else if (DirectionsSelected)
	{
		state = ((Down << 3) | (Up << 2) | (Left << 1) | Right);
	}

	return state;
}

void Joypad::WriteJoyPad(int value)
{
	ButtonsSelected = ((value & 0b00100000) == 0);
	DirectionsSelected = ((value & 0b00010000) == 0);
}

void Joypad::ProcessJoypadInput(Interrupts *interrupts, CPU *cpu)
{
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	SDL_PumpEvents();

	bool shouldRequestInterrupt = false;
	shouldRequestInterrupt |= ProcessKeyInput(&A, keys[SDL_SCANCODE_A]);
	shouldRequestInterrupt |= ProcessKeyInput(&B, keys[SDL_SCANCODE_S]);
	shouldRequestInterrupt |= ProcessKeyInput(&Start, keys[SDL_SCANCODE_RETURN]);
	shouldRequestInterrupt |= ProcessKeyInput(&Select, keys[SDL_SCANCODE_BACKSPACE]);
	shouldRequestInterrupt |= ProcessKeyInput(&Right, keys[SDL_SCANCODE_RIGHT]);
	shouldRequestInterrupt |= ProcessKeyInput(&Left, keys[SDL_SCANCODE_LEFT]);
	shouldRequestInterrupt |= ProcessKeyInput(&Up, keys[SDL_SCANCODE_UP]);
	shouldRequestInterrupt |= ProcessKeyInput(&Down, keys[SDL_SCANCODE_DOWN]);

	if (shouldRequestInterrupt)
	{
		interrupts->RequestJoypadInterrupt();
		cpu->NotifyInterruptOccurred();
	}
}

bool Joypad::ProcessKeyInput(char *key, Uint8 keyState)
{
	int previousKeyState = *key;
	*key = 1 - keyState;
	if (previousKeyState == 1 && A == 0)
	{
		return true;
	}

	return false;
}
