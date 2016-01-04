#include "stdafx.h"
#include <fstream>

#include "MMU.h"


MMU::MMU()
{
}

MMU::~MMU()
{
}

int MMU::ReadByte(long address)
{
	return 0;
}

int MMU::ReadWord(long address)
{
	return 0;
}

void MMU::WriteByte(int value, long address)
{
	return;
}

void MMU::WriteWord(int value, long address)
{
	return;
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
