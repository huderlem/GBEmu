#pragma once

#include <string>

class Battery
{
public:
	Battery(std::string saveDirectory, std::string ROMName);
	~Battery();
	void SaveRAM(const unsigned char *SRAM, long SRAMSize);
	unsigned char * LoadRAM(long SRAMSize);
private:
	std::string saveDirectory;
	std::string romName;
};
