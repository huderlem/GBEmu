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
	unsigned char *tilePixels = new unsigned char[8 * 8 * 0x180]();
	unsigned char *tileRAM;
	unsigned char *bgMapRAM;
	unsigned char *oamData;

	// Monochrome GameBoy palette registers
	void WriteBGP(int value);
	int BGP = 0;   // I/O register FF47
	unsigned char *BGPMap;
	void WriteOBP0(int value);
	int OBP0 = 0;  // I/O register FF48
	unsigned char *OBP0Map;
	void WriteOBP1(int value);
	int OBP1 = 0;  // I/O register FF49
	unsigned char *OBP1Map;
private:
	void InitializeVRAM();
	void UpdateTile(int tileId, int row, int lowByte, int highByte);
};
