#pragma once
#include "BaseMBC.h"
#include "Battery.h"

class MBC5 : public BaseMBC
{
public:
	MBC5(unsigned char *ROM, long ROMSize, int RAMSizeType, bool battery, std::string saveDirectory, std::string romName);
	~MBC5();
	virtual int ReadByteROMSwitchableBank(long address);
	virtual int ReadByteRAMSwitchableBank(long address);
	virtual void WriteByteSection0(int value, long address);
	virtual void WriteByteSection1(int value, long address);
	virtual void WriteByteSection2(int value, long address);
	virtual void WriteByteSection3(int value, long address);
	virtual void WriteByteRAMSwitchableBank(int value, long address);
	virtual void InitializeSRAM(int RAMSizeType);
	virtual void ExitGame();
	static long GetRAMSize(int RAMSizeType);
private:
	int ROMBankLo;
	int ROMBankHi;
	int RAMBank;

	bool RAMEnable;

	Battery *battery;
};