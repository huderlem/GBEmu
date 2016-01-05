#include "stdafx.h"
#include "Timer.h"


Timer::Timer()
{
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
		speed = tickSpeeds[(value & 3)];
		TAC = value;
	}
}
