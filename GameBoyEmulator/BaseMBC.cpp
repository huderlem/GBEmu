#include "stdafx.h"
#include "BaseMBC.h"

BaseMBC::BaseMBC(unsigned char *ROM, long ROMSizeType, bool batteryEnabled, std::string saveDirectory, std::string romName)
{
	BaseMBC::ROM = ROM;
	BaseMBC::ROMSizeType = ROMSizeType;
	BaseMBC::saveDirectory = saveDirectory;
	BaseMBC::romName = romName;
	InitializeSRAM();
	BaseMBC::batteryEnabled = batteryEnabled;
}

BaseMBC::~BaseMBC()
{
	//TODO: delete members ??
}

int BaseMBC::ReadByteROM0(long address)
{
	return ROM[address];
}

int BaseMBC::ReadByteROMSwitchableBank(long address)
{
	return ROM[address];
}

int BaseMBC::ReadByteRAMSwitchableBank(long address)
{
	int offset = address - 0xA000;

	if (offset < SRAMSize)
	{
		return SRAM[offset];
	}
	else
	{
		return 0;
	}
}

void BaseMBC::WriteByteSection0(int value, long address)
{
}

void BaseMBC::WriteByteSection1(int value, long address)
{
}

void BaseMBC::WriteByteSection2(int value, long address)
{
}

void BaseMBC::WriteByteSection3(int value, long address)
{
}

void BaseMBC::WriteByteRAMSwitchableBank(int value, long address)
{
	long offset = address - 0xA000;
	SRAM[offset] = (unsigned char)value;
}

void BaseMBC::InitializeSRAM()
{
	SRAMSize = 0x2000;
	SRAM = new unsigned char[SRAMSize]();
}

void BaseMBC::ExitGame()
{
}

void BaseMBC::Tick(int cpucycles, int cpuCycles)
{
}

long BaseMBC::GetRAMSize(int RAMSizeType)
{
	switch (RAMSizeType)
	{
	case 0x00:
		return 0;
	case 0x01:
		return 0x800;
	case 0x02:
		return 0x2000;
	case 0x03:
		return 0x8000;
	default:
		return 0;
	}
}

void BaseMBC::BatterySave(const unsigned char * SRAM, long SRAMSize)
{
	std::string saveFilepath = saveDirectory + romName + ".sav";

	std::ofstream saveFile;
	saveFile.open(saveFilepath, std::ios::out | std::ios::binary | std::ios::trunc);
	if (saveFile.is_open())
	{
		saveFile.write((char *)SRAM, SRAMSize);
		saveFile.close();
	}
	else
	{
		// TODO: handle save file error.
	}
}

unsigned char * BaseMBC::BatteryLoad(long SRAMSize)
{
	std::string saveFilepath = saveDirectory + romName + ".sav";

	std::fstream saveFile;
	saveFile.open(saveFilepath, std::ios::in | std::ios::binary);
	if (saveFile.is_open())
	{
		long start = (long)saveFile.tellg();
		saveFile.seekg(0, std::ios::end);
		long end = (long)saveFile.tellg();
		long SRAMFileSize = end - start;

		unsigned char * saveRAM = new unsigned char[SRAMSize];
		saveFile.seekg(0, std::ios::beg);
		saveFile.read((char *)saveRAM, SRAMSize);

		saveFile.close();
		return saveRAM;
	}
	else
	{
		return nullptr;
	}
}
