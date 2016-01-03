#include "stdafx.h"
#include "CPURegisters.h"

CPURegisters::CPURegisters()
{
	// Initialize all registers to 0.
	a = 0;
	f = 0;
	b = 0;
	c = 0;
	d = 0;
	e = 0;
	h = 0;
	l = 0;
	pc = 0;
	sp = 0;
}

CPURegisters::~CPURegisters()
{
}
