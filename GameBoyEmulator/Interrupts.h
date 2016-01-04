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
	void SetInterruptEnableRegister(int value);
	int GetInterruptEnableRegister();
private:
	// Interrupt Master Enable flag
	bool IME;

	// FFFF: IE - Interrupt Enable Register
	// Bit 0 : VBlank   Interrupt Enable(INT 40h)  (1 = Enable)
	// Bit 1 : LCD STAT Interrupt Enable(INT 48h)  (1 = Enable)
	// Bit 2 : Timer    Interrupt Enable(INT 50h)  (1 = Enable)
	// Bit 3 : Serial   Interrupt Enable(INT 58h)  (1 = Enable)
	// Bit 4 : Joypad   Interrupt Enable(INT 60h)  (1 = Enable)
	int IE;
};

