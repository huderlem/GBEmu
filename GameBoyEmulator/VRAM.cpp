#include "stdafx.h"
#include "VRAM.h"


VRAM::VRAM()
{
	InitializeVRAM();
}

VRAM::~VRAM()
{
}

int VRAM::ReadByte(long address)
{
	if (address < 0x9800)
	{
		long offset = address - 0x8000;
		return tileRAM[offset];
	}
	else
	{
		long offset = address - 0x9800;
		return bgMapRAM[offset];
	}
}

void VRAM::WriteByte(int value, long address)
{
	if (address < 0x9800)
	{
		long offset = address - 0x8000;
		tileRAM[offset] = (unsigned char)value;
	}
	else
	{
		long offset = address - 0x9800;
		bgMapRAM[offset] = (unsigned char)value;
	}
}

void VRAM::InitializeVRAM()
{
	tileRAM = new unsigned char[0x1800]();
	bgMapRAM= new unsigned char[0x800]();
}
