#include "stdafx.h"
#include "BaseMBC.h"

BaseMBC::BaseMBC(unsigned char *ROM, long ROMSize)
{
	BaseMBC::ROM = ROM;
	BaseMBC::ROMSize = ROMSize;
	InitializeSRAM();
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
	int offset = (address - 0xa000);
	return SRAM[offset];
}

void BaseMBC::InitializeSRAM()
{
	SRAMSize = 0x2000;
	SRAM = new unsigned char[SRAMSize]();
}
