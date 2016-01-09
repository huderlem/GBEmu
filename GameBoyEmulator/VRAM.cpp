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
	else if (address < 0xA000)
	{
		long offset = address - 0x9800;
		bgMapRAM[offset] = (unsigned char)value;
	}
}

int VRAM::ReadByteOAM(long address)
{
	long offset = address - 0xFE00;
	return oamData[offset];
}

void VRAM::WriteByteOAM(int value, long address)
{
	long offset = address - 0xFE00;
	oamData[offset] = (unsigned char)value;
}

void VRAM::InitializeVRAM()
{
	tileRAM = new unsigned char[0x1800]();
	bgMapRAM= new unsigned char[0x800]();
	oamData = new unsigned char[0xA0]();
}
