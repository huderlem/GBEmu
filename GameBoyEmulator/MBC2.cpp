#include "stdafx.h"
#include "MBC2.h"

MBC2::MBC2(unsigned char * ROM, long ROMSizeType, int RAMSizeType, bool batteryEnabled, std::string saveDirectory, std::string romName) : BaseMBC(ROM, ROMSizeType, batteryEnabled)
{
	ROMBank = 1;
	RAMEnable = false;
	battery = new Battery(saveDirectory, romName);
	InitializeSRAM(RAMSizeType);
}

MBC2::~MBC2()
{
}

int MBC2::ReadByteROMSwitchableBank(long address)
{
	int bankOffset = ROMBank * 0x4000;
	long trueAddress = bankOffset + (address - 0x4000);

	return ROM[trueAddress];
}

int MBC2::ReadByteRAMSwitchableBank(long address)
{
	// 512x4-bit RAM.  Not a switchable bank.
	if (address > 0xA1FF)
	{
		// TODO: How to treat this out of bounds access?
		return 0;
	}

	long trueAddress = address - 0xA000;

	// Only lower 4 bits are used in this MBC chip.
	return (SRAM[trueAddress] & 0x0F);
}

void MBC2::WriteByteSection0(int value, long address)
{
	// RAM enable
	if ((address & 0x100) > 0)
	{
		// Least significant bit of high byte must be 0 to enable/disable RAM.
		//return;
	}

	// TODO: is it supposed be == 0x0A, like MCB1?
	if (value == 0)
	{
		RAMEnable = false;
	}
	else
	{
		RAMEnable = true;
	}
}

void MBC2::WriteByteSection1(int value, long address)
{
	// ROM Bank Number
	if ((address & 0x100) == 0)
	{
		// Least significant bit of high byte must be 1 to select ROM bank.
		return;
	}

	// Only lower 4 bits are used.
	ROMBank = (value & 0x0F);
}

void MBC2::WriteByteSection2(int value, long address)
{
}

void MBC2::WriteByteSection3(int value, long address)
{
}

void MBC2::WriteByteRAMSwitchableBank(int value, long address)
{
	long trueAddress = address - 0xA000;

	// Only lower 4 bits are used in this MBC chip.
	SRAM[trueAddress] = (value & 0x0F);
}

void MBC2::InitializeSRAM(int RAMSizeType)
{
	SRAMSize = 0x200;
	if (batteryEnabled)
	{
		SRAM = battery->LoadRAM(SRAMSize);
	}

	if (SRAM == nullptr)
	{
		// TODO: fill with random values, instead of 0.
		SRAM = new unsigned char[SRAMSize]();
	}
}

void MBC2::ExitGame()
{
	if (batteryEnabled)
	{
		battery->SaveRAM(SRAM, SRAMSize);
	}
}
