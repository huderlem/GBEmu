#include "stdafx.h"
#include "MockMMU.h"

MockMMU::MockMMU(int byte, int word)
{
	MockMMU::byte = byte;
	MockMMU::word = word;
}

MockMMU::MockMMU()
{
}

MockMMU::~MockMMU()
{
}

int MockMMU::ReadByte(long address)
{
	return byte;
}

int MockMMU::ReadWord(long address)
{
	return word;
}

void MockMMU::WriteByte(int value, long address)
{
	return;
}

void MockMMU::WriteWord(int value, long address)
{
	return;
}

bool MockMMU::LoadROM(std::string filepath)
{
	return true;
}
