#include "stdafx.h"
#include "Joypad.h"


Joypad::Joypad()
{
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
