#include "stdafx.h"
#include "MBC3.h"


MBC3::MBC3(unsigned char * ROM, long ROMSizeType, int RAMSizeType, bool batteryEnabled, std::string saveDirectory, std::string romName) : BaseMBC(ROM, ROMSizeType, batteryEnabled)
{
	ROMBank = 1;
	RAM_RTC_Enable = false;
	RAM_RTC_Register = 0;
	battery = new Battery(saveDirectory, romName);
	InitializeSRAM(RAMSizeType);
}

MBC3::~MBC3()
{
}

int MBC3::ReadByteROMSwitchableBank(long address)
{
	int bankOffset = ROMBank * 0x4000;
	long trueAddress = bankOffset + (address - 0x4000);

	return ROM[trueAddress];
}

int MBC3::ReadByteRAMSwitchableBank(long address)
{
	if (!RAM_RTC_Enable)
	{
		return 0;
	}

	if (RAM_RTC_Register >= 0 && RAM_RTC_Register <= 3)
	{
		// RAM bank.
		int ramOffset = RAM_RTC_Register * 0x2000;
		int trueAddress = ramOffset + (address - 0xA000);
		return SRAM[trueAddress];
	}
	else if (RAM_RTC_Register >= 0x8 && RAM_RTC_Register <= 0xC)
	{
		// RTC Register.
		/*time_t t = time(0);
		struct tm *now = localtime(&t);
		switch (RAM_RTC_Register)
		{
		case 0x8:
			return now->tm_sec;
		case 0x9:
			return now->tm_min;
		case 0xA:
			return now->tm_hour;
		case 0xB:
			return now->
		}*/
		// TODO:
		return 0;
	}

	return 0;
}

void MBC3::WriteByteSection0(int value, long address)
{
	if ((value & 0x0f) == 0x0A)
	{
		RAM_RTC_Enable = true;
	}
	else
	{
		RAM_RTC_Enable = false;
	}
}

void MBC3::WriteByteSection1(int value, long address)
{
	if (value > 0x7f)
	{
		int a = 0;
	}

	if (value == 0)
	{
		ROMBank = 1;
	}
	else
	{
		// Only 0x7F banks are supported in MBC3.
		ROMBank = (value & 0x7f);
	}
}

void MBC3::WriteByteSection2(int value, long address)
{
	if ((value >= 0x0 && value <= 0x3) || (value >= 0x8 && value <= 0xC))
	{
		// Selecting either RAM Bank or Real-time Clock register.
		RAM_RTC_Register = value;
	}
}

void MBC3::WriteByteSection3(int value, long address)
{
	// TODO:
}

void MBC3::WriteByteRAMSwitchableBank(int value, long address)
{
	if (!RAM_RTC_Enable)
	{
		return;
	}

	if (RAM_RTC_Register >= 0 && RAM_RTC_Register <= 3)
	{
		// RAM bank.
		int ramOffset = RAM_RTC_Register * 0x2000;
		int trueAddress = ramOffset + (address - 0xA000);
		SRAM[trueAddress] = value;
	}
	else if (RAM_RTC_Register >= 0x8 && RAM_RTC_Register <= 0xC)
	{
		// RTC Register.
		/*time_t t = time(0);
		struct tm *now = localtime(&t);
		switch (RAM_RTC_Register)
		{
		case 0x8:
		return now->tm_sec;
		case 0x9:
		return now->tm_min;
		case 0xA:
		return now->tm_hour;
		case 0xB:
		return now->
		}*/
		// TODO:
	}
}

void MBC3::InitializeSRAM(int RAMSizeType)
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

void MBC3::ExitGame()
{
	if (batteryEnabled)
	{
		battery->SaveRAM(SRAM, SRAMSize);
	}
}
