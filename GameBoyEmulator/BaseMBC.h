#pragma once
#include <string>
#include <iostream>
#include <fstream>

class BaseMBC
{
public:
	BaseMBC(unsigned char *ROM, long ROMSize, bool batteryEnabled, std::string saveDirectory, std::string romName);
	~BaseMBC();
	int ReadByteROM0(long address);
	virtual int ReadByteROMSwitchableBank(long address);
	virtual int ReadByteRAMSwitchableBank(long address);
	virtual void WriteByteSection0(int value, long address);
	virtual void WriteByteSection1(int value, long address);
	virtual void WriteByteSection2(int value, long address);
	virtual void WriteByteSection3(int value, long address);
	virtual void WriteByteRAMSwitchableBank(int value, long address);
	virtual void InitializeSRAM();
	virtual void ExitGame();
	virtual void Tick(int cpucycles, int cpuCycles);
	static long GetRAMSize(int RAMSizeType);
	virtual void BatterySave(const unsigned char *SRAM, long SRAMSize);
	virtual unsigned char * BatteryLoad(long SRAMSize);
protected:
	unsigned char *ROM;
	long ROMSizeType;
	unsigned char *SRAM;
	long SRAMSize;

	bool batteryEnabled;
	std::string saveDirectory;
	std::string romName;
};
