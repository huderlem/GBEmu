#include "stdafx.h"
#include "WRAM.h"


WRAM::WRAM()
{
	InitializeRAM();
}

WRAM::~WRAM()
{
}

int WRAM::ReadByte(long address)
{
	int offset = address - 0xC000;
	return RAM[offset];
}

void WRAM::WriteByte(int value, long address)
{
	int offset = address - 0xC000;
	RAM[offset] = (unsigned char)value;
}

void WRAM::InitializeRAM()
{
	RAMSize = 0x2000;
	RAM = new unsigned char[RAMSize]();
}
