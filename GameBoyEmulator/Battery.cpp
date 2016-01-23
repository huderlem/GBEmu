#include "stdafx.h"
#include <iostream>
#include <fstream>

#include "Battery.h"


Battery::Battery(std::string saveDirectory, std::string ROMName)
{
	this->saveDirectory = saveDirectory;
	romName = ROMName;
}

Battery::~Battery()
{
}

void Battery::SaveRAM(const unsigned char *SRAM, long SRAMSize)
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

unsigned char * Battery::LoadRAM(long SRAMSize)
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
		saveFile.read((char *)saveRAM, SRAMFileSize);

		// Fill any remaining SRAM space with 0.
		char zero = 0;
		for (long i = SRAMFileSize; i < SRAMSize; i++)
		{
			saveFile.write(&zero, sizeof(char));
		}

		saveFile.close();
		return saveRAM;
	}
	else
	{
		return nullptr;
	}
}
