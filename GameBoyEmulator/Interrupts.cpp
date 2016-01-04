#include "stdafx.h"
#include "Interrupts.h"


Interrupts::Interrupts()
{
	IME = true;
}


Interrupts::~Interrupts()
{
}

void Interrupts::DisableInterrupts()
{
	IME = false;
}

void Interrupts::EnableInterrupts()
{
	IME = true;
}

bool Interrupts::InterruptsEnabled()
{
	return IME;
}

void Interrupts::SetInterruptEnableRegister(int value)
{
	IE = value;
}

int Interrupts::GetInterruptEnableRegister()
{
	return IE;
}
