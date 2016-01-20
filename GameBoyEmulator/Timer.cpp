#include "stdafx.h"
#include "Timer.h"


int Timer::TickSpeeds[4] = { 1024, 16, 64, 256 };

Timer::Timer()
{
	DIV = 0;
	TicksDIV = 0;
	TIMA = 0;
	TicksTIMA = 0;
}


Timer::~Timer()
{
}

int Timer::ReadByte(long address)
{
	switch (address)
	{
	case 0xFF04:
		return DIV;
	case 0xFF05:
		return TIMA;
	case 0xFF06:
		return TMA;
	case 0xFF07:
		return TAC;
	}

	// TODO: Throw exception?
	return 0;
}

void Timer::WriteByte(int value, long address)
{
	switch (address)
	{
	case 0xFF04:
		DIV = 0;
		break;
	case 0xFF05:
		TIMA = value;
		break;
	case 0xFF06:
		TMA = value;
		break;
	case 0xFF07:
		enabled = ((value & 0b00000100) > 0);
		speed = TickSpeeds[(value & 3)];
		TAC = value;
	}
}

void Timer::Tick(int cpuCycles, Interrupts *interrupts, CPU *cpu)
{
	// DIV register increments every 256 cpu cycles (16384 Hz).
	TicksDIV += cpuCycles;
	while (TicksDIV >= 256)
	{
		TicksDIV -= 256;
		DIV = ((DIV + 1) & 0xFF);
	}

	// TIMA register increments at an interval determined by the TAC register.
	if (enabled)
	{
		TicksTIMA += cpuCycles;
		while (TicksTIMA >= speed)
		{
			TicksTIMA -= speed;
			TIMA++;
			if (TIMA > 0xFF)
			{
				TIMA = TMA;
				interrupts->RequestTimerInterrupt();
				cpu->NotifyInterruptOccurred();
			}
		}
	}
}
