#pragma once
#include <string>

#include "BaseMBC.h"
#include "Battery.h"

class MBC2 : public BaseMBC
{
public:
	MBC2(unsigned char *ROM, long ROMSize, int RAMSizeType, bool battery, std::string saveDirectory, std::string romName);
	~MBC2();
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
	bool RAMEnable;

	Battery *battery;
};

