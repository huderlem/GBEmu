#pragma once
class WRAM
{
public:
	WRAM();
	~WRAM();
	virtual int ReadByte(long address);
	virtual void WriteByte(int value, long address);
protected:
	virtual void InitializeRAM();
	unsigned char *RAM;
	long RAMSize;
};
