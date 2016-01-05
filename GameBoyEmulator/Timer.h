#pragma once
class Timer
{
public:
	Timer();
	~Timer();
	int ReadByte(long address);
private:
	// I/O Register FF04 - Divider Register
	int DIV = 0;
	// I/O Register FF05 - Timer Counter
	int TIMA = 0;
	// I/O Register FF06 - Timer Modulo
	int TMA = 0;
	// I/O Register FF07 - Timer Control
	int TAC = 0;
};
