#pragma once
#include <string>
#include <ctime>

#include "BaseMBC.h"
#include "Battery.h"

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
private:
	int ROMBank;
	int RAM_RTC_Register;

	bool RAM_RTC_Enable;

	Battery *battery;
};

