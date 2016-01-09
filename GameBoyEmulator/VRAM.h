#pragma once
class VRAM
{
public:
	VRAM();
	~VRAM();
	int ReadByte(long address);
	void WriteByte(int value, long address);
	int ReadByteOAM(long address);
	void WriteByteOAM(int value, long address);
private:
	unsigned char *tileRAM;
	unsigned char *bgMapRAM;
	unsigned char *oamData;
	void InitializeVRAM();
};
