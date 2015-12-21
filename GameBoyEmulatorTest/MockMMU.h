#pragma once

#include "stdafx.h"
#include "../GameBoyEmulator/IMMU.h"

class MockMMU : public IMMU
{
public:
	MockMMU(int byte, int word);
	MockMMU();
	~MockMMU();
	int byte;
	int word;
	virtual int ReadByte(long address);
	virtual int ReadWord(long address);
	virtual void WriteByte(int value, long address);
	virtual void WriteWord(int value, long address);
};

