#pragma once

#include <string>

#define TEST_API __declspec(dllexport)

class TEST_API IMMU
{
public:
	virtual int ReadByte(long address) =0;
	virtual int ReadWord(long address) =0;
	virtual void WriteByte(int value, long address) =0;
	virtual void WriteWord(int value, long address) =0;
	virtual bool LoadROM(std::string filepath) =0;
	virtual void InitializeMBC() =0;
};

