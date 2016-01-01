#pragma once

#define TEST_API __declspec(dllexport)

class TEST_API Interrupts
{
public:
	Interrupts();
	~Interrupts();
	void DisableInterrupts();
	void EnableInterrupts();
	bool InterruptsEnabled();
private:
	bool IME;  // Interrupt Master Enable flag
};

