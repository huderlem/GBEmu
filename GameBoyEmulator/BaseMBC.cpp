#include "stdafx.h"
#include "BaseMBC.h"

BaseMBC::BaseMBC(unsigned char *ROM, long ROMSize, bool battery)
{
	BaseMBC::ROM = ROM;
	BaseMBC::ROMSize = ROMSize;
	InitializeSRAM();
	BaseMBC::battery = battery;
}

BaseMBC::~BaseMBC()
{
	//TODO: delete members ??
}

int BaseMBC::ReadByteROM0(long address)
{
	return ROM[address];
}

int BaseMBC::ReadByteROMSwitchableBank(long address)
{
	return ROM[address];
}

int BaseMBC::ReadByteRAMSwitchableBank(long address)
{
	int offset = address - 0xA000;

	if (offset < SRAMSize)
	{
		return SRAM[offset];
	}
	else
	{
		return 0;
	}
}

void BaseMBC::WriteByteSection0(int value, long address)
{
}

void BaseMBC::WriteByteSection1(int value, long address)
{
}

void BaseMBC::WriteByteSection2(int value, long address)
{
}

void BaseMBC::WriteByteSection3(int value, long address)
{
}

void BaseMBC::WriteByteRAMSwitchableBank(int value, long address)
{
	long offset = address - 0xA000;
	SRAM[offset] = (unsigned char)value;
}

void BaseMBC::InitializeSRAM()
{
	SRAMSize = 0x2000;
	SRAM = new unsigned char[SRAMSize]();
}

long BaseMBC::GetRAMSize(int RAMSizeType)
{
	switch (RAMSizeType)
	{
	case 0x00:
		return 0;
	case 0x01:
		return 0x800;
	case 0x02:
		return 0x2000;
	case 0x03:
		return 0x8000;
	default:
		return 0;
	}
}
