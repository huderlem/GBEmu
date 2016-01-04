#pragma once

#include "IMMU.h"

class MMU: public IMMU
{
public:
	MMU();
	~MMU();
	virtual int ReadByte(long address);
	virtual int ReadWord(long address);
	virtual void WriteByte(int value, long address);
	virtual void WriteWord(int value, long address);
	virtual bool LoadROM(std::string filepath);
private:
	// Byte array of entire ROM.
	unsigned char *ROM;
	long ROMSize;
};

