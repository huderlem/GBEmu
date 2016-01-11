#include "stdafx.h"
#include "VRAM.h"


VRAM::VRAM()
{
	InitializeVRAM();
}

VRAM::~VRAM()
{
}

int VRAM::ReadByte(long address)
{
	if (address < 0x9800)
	{
		long offset = address - 0x8000;
		return tileRAM[offset];
	}
	else
	{
		long offset = address - 0x9800;
		return bgMapRAM[offset];
	}
}

void VRAM::WriteByte(int value, long address)
{
	if (address < 0x9800)
	{
		long offset = address - 0x8000;
		tileRAM[offset] = (unsigned char)value;
		int tileId = offset / 0x10;
		int row = (offset % 0x10) / 2;
		int lowByte = (offset % 2) == 0 ? tileRAM[offset] : tileRAM[offset - 1];
		int highByte = (offset % 2) == 0 ? tileRAM[offset + 1] : tileRAM[offset];
		UpdateTile(tileId, row, lowByte, highByte);
	}
	else if (address < 0xA000)
	{
		long offset = address - 0x9800;
		bgMapRAM[offset] = (unsigned char)value;
	}
}

void VRAM::UpdateTile(int tileId, int row, int lowByte, int highByte)
{
	for (int i = 0; i < 8; i++)
	{
		int lowBit = (lowByte >> (7 - i)) & 1;
		int highBit = (highByte >> (7 - i)) & 1;
		int newPixel = (highBit << 1) | lowBit;
		int pixelIndex = (tileId * 64) + (row * 8) + i;
		tilePixels[pixelIndex] = newPixel;
	}
}

int VRAM::ReadByteOAM(long address)
{
	long offset = address - 0xFE00;
	return oamData[offset];
}

void VRAM::WriteByteOAM(int value, long address)
{
	long offset = address - 0xFE00;
	oamData[offset] = (unsigned char)value;
}

void VRAM::WriteBGP(int value)
{
	BGP = value;
	BGPMap[3] = (value >> 6) & 0b11;
	BGPMap[2] = (value >> 4) & 0b11;
	BGPMap[1] = (value >> 2) & 0b11;
	BGPMap[0] = value & 0b11;
}

void VRAM::WriteOBP0(int value)
{
	OBP0 = value;
	OBP0Map[3] = (value >> 6) & 0b11;
	OBP0Map[2] = (value >> 4) & 0b11;
	OBP0Map[1] = (value >> 2) & 0b11;
	OBP0Map[0] = value & 0b11;
}

void VRAM::WriteOBP1(int value)
{
	OBP1 = value;
	OBP1Map[3] = (value >> 6) & 0b11;
	OBP1Map[2] = (value >> 4) & 0b11;
	OBP1Map[1] = (value >> 2) & 0b11;
	OBP1Map[0] = value & 0b11;
}

void VRAM::InitializeVRAM()
{
	tileRAM = new unsigned char[0x1800]();
	bgMapRAM= new unsigned char[0x800]();
	oamData = new unsigned char[0xA0]();
	BGPMap = new unsigned char[4]();
	OBP0Map = new unsigned char[4]();
	OBP1Map = new unsigned char[4]();
}
