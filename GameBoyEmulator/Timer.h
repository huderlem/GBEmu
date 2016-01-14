#pragma once
#include "Interrupts.h"

class Timer
{
public:
	Timer();
	~Timer();
	int ReadByte(long address);
	void WriteByte(int value, long address);
	void Tick(int cpuCycles, Interrupts *interrupts);
private:
	// I/O Register FF04 - Divider Register
	int DIV;
	int TicksDIV;

	// I/O Register FF05 - Timer Counter
	int TIMA;
	int TicksTIMA;

	// I/O Register FF06 - Timer Modulo
	int TMA = 0;

	// I/O Register FF07 - Timer Control
	int TAC = 0;
	int enabled = false;
	int speed = 1;
	static int TickSpeeds[4];
};

