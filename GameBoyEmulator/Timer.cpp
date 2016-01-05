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
