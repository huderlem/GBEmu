#pragma once
class VRAM
{
public:
	VRAM();
	~VRAM();
	int ReadByte(long address);
	void WriteByte(int value, long address);
private:
	unsigned char *tileRAM;
	unsigned char *bgMapRAM;
	void InitializeVRAM();
};
