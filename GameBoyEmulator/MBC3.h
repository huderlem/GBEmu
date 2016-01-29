#pragma once
#include <string>
#include <time.h>

#include "BaseMBC.h"

class MBC3 : public BaseMBC
{
public:
	MBC3(unsigned char *ROM, long ROMSize, int RAMSizeType, bool battery, std::string saveDirectory, std::string romName);
	~MBC3();
	virtual int ReadByteROMSwitchableBank(long address);
	virtual int ReadByteRAMSwitchableBank(long address);
	virtual void WriteByteSection0(int value, long address);
	virtual void WriteByteSection1(int value, long address);
	virtual void WriteByteSection2(int value, long address);
	virtual void WriteByteSection3(int value, long address);
	virtual void WriteByteRAMSwitchableBank(int value, long address);
	virtual void InitializeSRAM(int RAMSizeType);
	virtual void ExitGame();
	virtual void Tick(int cpuCycles, int cyclesPerSecond);
	virtual void BatterySave(const unsigned char *SRAM, long SRAMSize);
	virtual unsigned char * BatteryLoad(long SRAMSize);
private:
	void InitializeRTC();
	void AdvanceRTCSeconds(int seconds);

	int ROMBank;
	int RAM_RTC_Register;
	int RTC_TickCounter;

	bool RAM_RTC_Enable;

	int RTC_TimerEnable;
	int RTC_Seconds;
	int RTC_Minutes;
	int RTC_Hours;
	int RTC_DaysLo;
	int RTC_DaysHi;
	int RTC_DayCarry;

	int RTC_Latched_Seconds;
	int RTC_Latched_Minutes;
	int RTC_Latched_Hours;
	int RTC_Latched_DaysLo;
	int RTC_Latched_DaysHi;

	bool latchStarted;
};

