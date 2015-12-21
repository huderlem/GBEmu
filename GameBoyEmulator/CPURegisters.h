#pragma once

#define TEST_API __declspec(dllexport)

class TEST_API CPURegisters
{
public:
	int a;
	int f;
	int b;
	int c;
	int d;
	int e;
	int h;
	int l;
	int sp;
	int pc;
	CPURegisters();
	~CPURegisters();
};
