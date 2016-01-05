#pragma once
class BaseMBC
{
public:
	BaseMBC(unsigned char *ROM, long ROMSize);
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
protected:
	unsigned char *ROM;
	long ROMSize;
	unsigned char *SRAM;
	long SRAMSize;
};
