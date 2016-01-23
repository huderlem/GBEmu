#include "stdafx.h"
#include "MBC1.h"


MBC1::MBC1(unsigned char *ROM, long ROMSizeType, int RAMSizeType, bool batteryEnabled, std::string saveDirectory, std::string romName) : BaseMBC(ROM, ROMSizeType, batteryEnabled)
{
	ModeSelect = 0;
	ExternalRAMEnable = false;
	battery = new Battery(saveDirectory, romName);
	InitializeSRAM(RAMSizeType);
}

MBC1::~MBC1()
{
	battery->SaveRAM(SRAM, SRAMSize);
}

int MBC1::ReadByteROMSwitchableBank(long address)
{
	int ROMBank = GetROMBank();

	// MBC1 quirk, in which Banks 0x00, 0x20, 0x40, 0x60 are mapped to 0x01, 0x21, 0x41, 0x61
	if (ROMBank == 0 || ROMBank == 0x20 || ROMBank == 0x40 || ROMBank == 0x60)
	{
		ROMBank++;
	}

	int bankOffset = ROMBank * 0x4000;
	long trueAddress = bankOffset + (address - 0x4000);

	return ROM[trueAddress];
}

int MBC1::ReadByteRAMSwitchableBank(long address)
{
	if (!ExternalRAMEnable)
	{
		return 0;
	}

	long ramAddress = GetRAMSwitchableBankAddress(address);
	if (ramAddress < SRAMSize)
	{
		return SRAM[ramAddress];
	}
	else
	{
		return 0;
	}
}

void MBC1::WriteByteSection0(int value, long address)
{
	if ((value & 0x0f) == 0x0A)
	{
		ExternalRAMEnable = true;
	}
	else
	{
		ExternalRAMEnable = false;
	}
}

void MBC1::WriteByteSection1(int value, long address)
{
	ROMBankLo = value & 0b00011111;
}

void MBC1::WriteByteSection2(int value, long address)
{
	RAMBank = value & 0b00000011;
	ROMBankHi = value & 0b00000011;
}

void MBC1::WriteByteSection3(int value, long address)
{
	ModeSelect = value & 0b00000001;
}

void MBC1::WriteByteRAMSwitchableBank(int value, long address)
{
	if (ExternalRAMEnable)
	{
		long ramAddress = GetRAMSwitchableBankAddress(address);
		SRAM[ramAddress] = value;
	}
}

void MBC1::InitializeSRAM(int RAMSizeType)
{
	SRAMSize = GetRAMSize(RAMSizeType);
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

void MBC1::ExitGame()
{
	if (batteryEnabled)
	{
		battery->SaveRAM(SRAM, SRAMSize);
	}
}

int MBC1::GetROMBank()
{
	if (ModeSelect == 0)
	{
		if (ROMSizeType > 4)
		{
			return (ROMBankHi << 5) | ROMBankLo;
		}
		else
		{
			return ROMBankLo;
		}
	}
	else
	{
		return ROMBankLo;
	}
}

long MBC1::GetRAMSwitchableBankAddress(long address)
{
	int bank = 0;
	if (ModeSelect == 1)
	{
		bank = RAMBank;
	}

	int ramOffset = bank * 0x2000;
	return ramOffset + (address - 0xA000);
}
