#include "stdafx.h"
#include <fstream>

#include "MMU.h"


MMU::MMU(Interrupts *interrupts)
{
	MMU::interrupts = interrupts;
}

MMU::~MMU()
{
}

int MMU::ReadByte(long address)
{
	// Determine which part of the memory map is being read.
	if (address < 0x4000)
	{
		// ROM Bank 0
		return mbc->ReadByteROM0(address);
	}
	else if (address < 0x8000)
	{
		// Switchable ROM Bank
		return mbc->ReadByteROMSwitchableBank(address);
	}
	else if (address < 0xA000)
	{
		// VRAM
		// TODO:
		return 0;
	}
	else if (address < 0xC000)
	{
		// External RAM
		return mbc->ReadByteRAMSwitchableBank(address);
	}
	else if (address < 0xE000)
	{
		// Working RAM
		// Bank 1 is switchable in Color GameBoy
		// TODO:
		return 0;
	}
	else if (address < 0xFE00)
	{
		// Echo of Working RAM (typically unused)
		int offset = address - 0x2000;
		// TODO:
		return 0;
	}
	else if (address < 0xFEA0)
	{
		// Sprite OAM table
		return 0;
	}
	else if (address < 0xFF00)
	{
		// Unusable section
		// TODO: throw exception?
		return 0;
	}
	else if (address < 0xff80)
	{
		// I/O registers
		// TODO:
		return 0;
	}
	else if (address < 0xFFFF)
	{
		// High RAM
		// TODO:
		return 0;
	}
	else if (address == 0xFFFF)
	{
		// Interrupt Enable Register
		return interrupts->GetInterruptEnableRegister();
	}

	// TODO: handle address out of bounds
	return 0;
}

// Reads a 2-byte little-endian value from the given memory address.
int MMU::ReadWord(long address)
{
	int lowByte = ReadByte(address);
	int highByte = ReadByte(address + 1);
	return ((highByte << 8) | lowByte);
}

void MMU::WriteByte(int value, long address)
{
	return;
}

// Writes a 2-byte little-endian value to the given memory address.
void MMU::WriteWord(int value, long address)
{
	int lowByte = (value & 0xff);
	int highByte = (value >> 8);
	WriteByte(lowByte, address);
	WriteByte(highByte, address + 1);
}

// Reads the given ROM file into memory.
// Returns whether or not the ROM was read successfully.
bool MMU::LoadROM(std::string filepath)
{
	bool success = true;
	try
	{
		std::ifstream romFile(filepath, std::ios::binary);
		if (romFile.is_open())
		{
			// Find size of ROM.
			long start = (long)romFile.tellg();
			romFile.seekg(0, std::ios::end);
			long end = (long)romFile.tellg();
			ROMSize = end - start;

			// Initialize ROM array, and read ROM contents into array.
			ROM = new unsigned char[ROMSize];
			romFile.seekg(0, std::ios::beg);
			romFile.read((char *)ROM, ROMSize);
			romFile.close();
		}
		else
		{
			// Unable to open ROM file.
			success = false;
		}
	}
	catch (...)
	{
		success = false;
	}

	return success;
}
