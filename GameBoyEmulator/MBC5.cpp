#include "stdafx.h"
#include "MBC5.h"


MBC5::MBC5(unsigned char * ROM, long ROMSizeType, int RAMSizeType, bool batteryEnabled, std::string saveDirectory, std::string romName) : BaseMBC(ROM, ROMSizeType, batteryEnabled, saveDirectory, romName)
{
	ROMBankLo = 1;
	ROMBankHi = 0;
	RAMBank = 0;
	RAMEnable = false;

	InitializeSRAM(RAMSizeType);
}


MBC5::~MBC5()
{
}

int MBC5::ReadByteROMSwitchableBank(long address)
{
	int ROMBank = (ROMBankHi << 8) | ROMBankLo;
	int bankOffset = ROMBank * 0x4000;
	long trueAddress = bankOffset + (address - 0x4000);
	
	return ROM[trueAddress];
}

int MBC5::ReadByteRAMSwitchableBank(long address)
{
	if (!RAMEnable)
	{
		return 0;
	}

	// RAM bank.
	int ramOffset = RAMBank * 0x2000;
	int trueAddress = ramOffset + (address - 0xA000);
	return SRAM[trueAddress];
}

void MBC5::WriteByteSection0(int value, long address)
{
	if ((value & 0x0f) == 0x0A)
	{
		RAMEnable = true;
	}
	else
	{
		RAMEnable = false;
	}
}

void MBC5::WriteByteSection1(int value, long address)
{
	if (address < 0x3000)
	{
		// ROM Bank Number low 8 bits
		ROMBankLo = value;
	}
	else
	{
		// ROM Bank Number high bit
		ROMBankHi = (value & 1);
	}
}

void MBC5::WriteByteSection2(int value, long address)
{
	// RAM Bank Number
	if (value < 0x10)
	{
		RAMBank = (value & 0x0F);
	}
}

void MBC5::WriteByteSection3(int value, long address)
{
}

void MBC5::WriteByteRAMSwitchableBank(int value, long address)
{
	// RAM bank.
	int ramOffset = RAMBank * 0x2000;
	int trueAddress = ramOffset + (address - 0xA000);
	SRAM[trueAddress] = value;
}

void MBC5::InitializeSRAM(int RAMSizeType)
{
	SRAMSize = GetRAMSize(RAMSizeType);
	if (batteryEnabled)
	{
		BatteryLoad(SRAMSize);
	}

	if (SRAM == nullptr)
	{
		// TODO: fill with random values, instead of 0.
		SRAM = new unsigned char[SRAMSize]();
	}
}

void MBC5::ExitGame()
{
	if (batteryEnabled)
	{
		BatterySave(SRAM, SRAMSize);
	}
}

long MBC5::GetRAMSize(int RAMSizeType)
{
	switch (RAMSizeType)
	{
	case 0x00:
		return 0;
	case 0x01:
		return 0x2000;
	case 0x02:
		return 0x8000;
	case 0x03:
		return 0x20000;
	default:
		return 0;
	}
}
