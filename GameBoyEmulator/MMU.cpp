#include "stdafx.h"
#include <fstream>

#include "MMU.h"


MMU::MMU(Interrupts *interrupts, WRAM *wram, VRAM *vram, Joypad *joypad, Timer *timer, LCDDisplay *display)
{
	MMU::interrupts = interrupts;
	MMU::wram = wram;
	MMU::vram = vram;
	MMU::joypad = joypad;
	MMU::timer = timer;
	MMU::display = display;
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
		return vram->ReadByte(address);
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
		return vram->ReadByteOAM(address);
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
		switch (address)
		{
		case 0xFF40:
			return display->LCDC;
		case 0xFF41:
			return display->ReadSTAT();
		case 0xFF42:
			return display->SCY;
		case 0xFF43:
			return display->SCX;
		case 0xFF44:
			return display->LY;
		case 0xFF45:
			return display->LYC;
		case 0xFF46:
			// DMA transfer register is write-only.
			return 0;
		case 0xFF47:
			return vram->BGP;
		case 0xFF48:
			return vram->OBP0;
		case 0xFF49:
			return vram->OBP1;
		case 0xFF4A:
			return display->WY;
		case 0xFF4B:
			return display->WX;
		}

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
		vram->WriteByte(value, address);
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
		wram->WriteByte(value, address);
	}
	else if (address < 0xFE00)
	{
		// Echo of Working RAM (typically unused)
		long offset = address - 0x2000;
		wram->WriteByte(value, address);
	}
	else if (address < 0xFEA0)
	{
		// Sprite OAM table
		vram->WriteByteOAM(value, address);
	}
	else if (address < 0xFF00)
	{
		// Unusable section
		// TODO: throw exception?
	}
	else if (address == 0xFF00)
	{
		// Joypad
		joypad->WriteJoyPad(value);
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
		
		switch (address)
		{
		case 0xFF40:
			display->LCDC = value;
			break;
		case 0xFF41:
			display->WriteSTAT(value);
			break;
		case 0xFF42:
			display->SCY = value;
			break;
		case 0xFF43:
			display->SCX = value;
			break;
		case 0xFF44:
			// LY register is readonly
			break;
		case 0xFF45:
			display->LYC = value;
			break;
		case 0xFF46:
			DMATransfer(value);
			break;
		case 0xFF47:
			vram->WriteBGP(value);
			break;
		case 0xFF48:
			vram->WriteOBP0(value);
			break;
		case 0xFF49:
			vram->WriteOBP1(value);
			break;
		case 0xFF4A:
			display->WY = value;
			break;
		case 0xFF4B:
			display->WX = value;
			break;
		}
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

	if (success)
	{
		cartType = ROM[0x147];
		ROMSizeType = ROM[0x148];
		RAMSizeType = ROM[0x149];
		destinationCode = ROM[0x14A];
	}

	return success;
}

void MMU::InitializeMBC(std::string romDirectory, std::string romName)
{
	switch (cartType)
	{
	case 0x00:
		mbc = new BaseMBC(ROM, ROMSizeType, false);
		break;
	case 0x01:
		mbc = new MBC1(ROM, ROMSizeType, 0, false, romDirectory, romName);
		break;
	case 0x02:
		mbc = new MBC1(ROM, ROMSizeType, RAMSizeType, false, romDirectory, romName);
		break;
	case 0x03:
		mbc = new MBC1(ROM, ROMSizeType, RAMSizeType, true, romDirectory, romName);
		break;
	case 0x05:
		mbc = new MBC2(ROM, ROMSizeType, RAMSizeType, false, romDirectory, romName);
		break;
	case 0x06:
		mbc = new MBC2(ROM, ROMSizeType, RAMSizeType, true, romDirectory, romName);
		break;
	case 0x09:
		mbc = new MBC3(ROM, ROMSizeType, 0, true, romDirectory, romName);
		break;
	case 0x10:
		mbc = new MBC3(ROM, ROMSizeType, RAMSizeType, true, romDirectory, romName);
		break;
	case 0x11:
		mbc = new MBC3(ROM, ROMSizeType, 0, false, romDirectory, romName);
		break;
	case 0x12:
		mbc = new MBC3(ROM, ROMSizeType, RAMSizeType, false, romDirectory, romName);
		break;
	case 0x13:
		mbc = new MBC3(ROM, ROMSizeType, RAMSizeType, true, romDirectory, romName);
		break;
	case 0x19:
		mbc = new MBC5(ROM, ROMSizeType, 0, false, romDirectory, romName);
		break;
	case 0x1A:
		mbc = new MBC5(ROM, ROMSizeType, RAMSizeType, false, romDirectory, romName);
		break;
	case 0x1B:
		mbc = new MBC5(ROM, ROMSizeType, RAMSizeType, true, romDirectory, romName);
		break;
	}
}

void MMU::ExitGame()
{
	mbc->ExitGame();
}

void MMU::TickMBC(int cpuCycles, int cyclesPerSecond)
{
	mbc->Tick(cpuCycles, cyclesPerSecond);
}

void MMU::InitializeHRAM()
{
	HRAMSize = 0x7F;
	HRAM = new unsigned char[HRAMSize]();
}

void MMU::DMATransfer(int value)
{
	int sourceAddress = (value << 8);
	for (int i = 0; i < 0xA0; i++)
	{
		vram->oamData[i] = ReadByte(sourceAddress + i);
	}
}
