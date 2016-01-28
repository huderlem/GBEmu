#include "stdafx.h"
#include "MBC3.h"


MBC3::MBC3(unsigned char * ROM, long ROMSizeType, int RAMSizeType, bool batteryEnabled, std::string saveDirectory, std::string romName) : BaseMBC(ROM, ROMSizeType, batteryEnabled)
{
	ROMBank = 1;
	RAM_RTC_Enable = false;
	RAM_RTC_Register = 0;
	battery = new Battery(saveDirectory, romName);
	InitializeSRAM(RAMSizeType);
	InitializeRTC();
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
		switch (RAM_RTC_Register)
		{
		case 0x8:
			return RTC_Latched_Seconds;
		case 0x9:
			return RTC_Latched_Minutes;
		case 0xA:
			return RTC_Latched_Hours;
		case 0xB:
			return RTC_Latched_DaysLo;
		case 0xC:
			return RTC_Latched_DaysHi;
		}
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
	// Latch the RTC values into the RTC registers.
	if (value != 0 && value != 1)
	{
		// Must write 0x0, then 0x1 to latch.
		// latchStarted = true, after the first 0x0 is written.
		latchStarted = false;
		return;
	}

	if (value == 0)
	{
		latchStarted = true;
	}
	else if (value == 1 && !latchStarted)
	{
		latchStarted = false;
	}
	else
	{
		RTC_Latched_Seconds = RTC_Seconds;
		RTC_Latched_Minutes = RTC_Minutes;
		RTC_Latched_Hours = RTC_Hours;
		RTC_Latched_DaysLo = RTC_DaysLo;
		RTC_Latched_DaysHi = (RTC_DayCarry << 7) | (RTC_TimerEnable << 6)  | RTC_DaysHi;
	}
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
		switch (RAM_RTC_Register)
		{
		case 0x8:
			RTC_Seconds = value;
			break;
		case 0x9:
			RTC_Minutes = value;
			break;
		case 0xA:
			RTC_Hours = value;
			break;
		case 0xB:
			RTC_DaysLo = value;
			break;
		case 0xC:
			RTC_DaysHi = (value & 1);
			RTC_TimerEnable = (value & 0b01000000) >> 6;
			RTC_DayCarry = (value & 0b10000000) ;
			break;
		}
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

void MBC3::Tick(int cpuCycles, int cyclesPerSecond)
{
	if (RTC_TimerEnable == 1)
	{
		return;
	}

	RTC_TickCounter += cpuCycles;
	while (RTC_TickCounter > cyclesPerSecond)
	{
		RTC_TickCounter -= cyclesPerSecond;
		RTC_Seconds += 1;
		if (RTC_Seconds >= 60)
		{
			RTC_Seconds = 0;
			RTC_Minutes += 1;
			if (RTC_Minutes >= 60)
			{
				RTC_Minutes = 0;
				RTC_Hours += 1;
				if (RTC_Hours >= 24)
				{
					RTC_Hours = 0;
					RTC_DaysLo += 1;
					if (RTC_DaysLo >= 0x100)
					{
						RTC_DaysLo = 0;
						RTC_DaysHi = 1;
					}
				}
			}
		}
	}
}

void MBC3::InitializeRTC()
{
	RTC_TickCounter = 0;

	RTC_TimerEnable = 0;
	RTC_Seconds = 0;
	RTC_Minutes = 59;
	RTC_Hours = 23;
	RTC_DaysLo = 255;
	RTC_DaysHi = 0;
	RTC_DayCarry = 0;

	RTC_Latched_Seconds = 0;
	RTC_Latched_Minutes = 0;
	RTC_Latched_Hours = 0;
	RTC_Latched_DaysLo = 0;
	RTC_Latched_DaysHi = 0;

	latchStarted = false;
}
