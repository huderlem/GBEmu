#include "stdafx.h"
#include <fstream>

#include "MMU.h"


MMU::MMU(Interrupts *interrupts, WRAM *wram, Joypad *joypad, Timer *timer)
{
	MMU::interrupts = interrupts;
	MMU::wram = wram;
	MMU::joypad = joypad;
	MMU::timer = timer;
	InitializeHRAM();
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
		return wram->ReadByte(address);
	}
	else if (address < 0xFE00)
	{
		// Echo of Working RAM (typically unused)
		long offset = address - 0x2000;
		return wram->ReadByte(address);
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
	else if (address == 0xFF00)
	{
		// Joypad
		return joypad->ReadJoypad();
	}
	else if (address < 0xFF03)
	{
		// Serial Data Transfer
		// TODO:
		return 0;
	}
	else if (address == 0xFF03)
	{
		// Unused I/O register
		// TODO:
		return 0;
	}
	else if (address < 0xFF08)
	{
		return timer->ReadByte(address);
	}
	else if (address < 0xFF0F)
	{
		// Unused I/O register
		// TODO:
		return 0;
	}
	else if (address == 0xFF0F)
	{
		// Interrupt Flag
		return interrupts->GetInterruptRequestRegister();
	}
	else if (address < 0xFF40)
	{
		// Sound registers
		// TODO:
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
		long offset = address - 0xFF80;
		return HRAM[offset];
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
	// Determine which part of the memory map is being read.
	if (address < 0x2000)
	{
		mbc->WriteByteSection0(value, address);
	}
	else if (address < 0x4000)
	{
		mbc->WriteByteSection1(value, address);
	}
	else if (address < 0x6000)
	{
		mbc->WriteByteSection2(value, address);
	}
	else if (address < 0x8000)
	{
		mbc->WriteByteSection3(value, address);
	}
	else if (address < 0xA000)
	{
		// VRAM
		// TODO:
	}
	else if (address < 0xC000)
	{
		// External RAM
		mbc->WriteByteRAMSwitchableBank(value, address);
	}
	else if (address < 0xE000)
	{
		// Working RAM
		// Bank 1 is switchable in Color GameBoy
		return wram->WriteByte(value, address);
	}
	else if (address < 0xFE00)
	{
		// Echo of Working RAM (typically unused)
		long offset = address - 0x2000;
		return wram->WriteByte(value, address);
	}
	else if (address < 0xFEA0)
	{
		// Sprite OAM table
		// TODO:
	}
	else if (address < 0xFF00)
	{
		// Unusable section
		// TODO: throw exception?
	}
	else if (address == 0xFF00)
	{
		// Joypad
		return joypad->WriteJoyPad(value);
	}
	else if (address < 0xFF03)
	{
		// Serial Data Transfer
		// TODO:
	}
	else if (address == 0xFF03)
	{
		// Unused I/O register
		// TODO:
	}
	else if (address < 0xFF08)
	{
		timer->WriteByte(value, address);
	}
	else if (address < 0xFF0F)
	{
		// Unused I/O register
		// TODO:
	}
	else if (address == 0xFF0F)
	{
		// Interrupt Flag
		interrupts->SetInterruptRequestRegister(value);
	}
	else if (address < 0xFF40)
	{
		// Sound registers
		// TODO:
	}
	else if (address < 0xff80)
	{
		// I/O registers
		// TODO:
	}
	else if (address < 0xFFFF)
	{
		// High RAM
		long offset = address - 0xFF80;
		HRAM[offset] = (unsigned char)value;
	}
	else if (address == 0xFFFF)
	{
		// Interrupt Enable Register
		interrupts->SetInterruptEnableRegister(value);
	}

	// TODO: handle address out of bounds
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

void MMU::InitializeHRAM()
{
	HRAMSize = 0x7F;
	HRAM = new unsigned char[HRAMSize]();
}
