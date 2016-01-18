#pragma once

#include "BaseMBC.h"

class MBC1: public BaseMBC
{
public:
	MBC1(unsigned char *ROM, long ROMSize, int RAMSizeType, bool battery);
	~MBC1();
	virtual int ReadByteROMSwitchableBank(long address);
	virtual int ReadByteRAMSwitchableBank(long address);
	virtual void WriteByteSection0(int value, long address);
	virtual void WriteByteSection1(int value, long address);
	virtual void WriteByteSection2(int value, long address);
	virtual void WriteByteSection3(int value, long address);
	virtual void WriteByteRAMSwitchableBank(int value, long address);
	virtual void InitializeSRAM(int RAMSizeType);
private:
	int ROMBankLo;
	int ROMBankHi;
	int RAMBank;

	// 0 = ROM Banking Mode
	// 1 = RAM Banking Mode
	int ModeSelect;

	bool ExternalRAMEnable;

	int GetROMBank();
	long GetRAMSwitchableBankAddress(long address);
};

