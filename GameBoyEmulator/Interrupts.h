#pragma once

#define TEST_API __declspec(dllexport)

#include "CPURegisters.h"
#include "IMMU.h"

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
	void SetInterruptRequestRegister(int value);
	int GetInterruptRequestRegister();
	void RequestVBlankInterrupt();
	void RequestLCDStatInterrupt();
	void RequestTimerInterrupt();
	void RequestSerialInterrupt();
	void RequestJoypadInterrupt();
	void ExecutePendingInterrupt(CPURegisters *registers, IMMU *mmu);
private:
	// Interrupt Master Enable flag
	bool IME;

	// FF0F: IF - Interrupt Flag
	// Bit 0 : V - Blank  Interrupt Request(INT 40h)  (1 = Request)
	// Bit 1 : LCD STAT Interrupt Request(INT 48h)    (1 = Request)
	// Bit 2 : Timer    Interrupt Request(INT 50h)    (1 = Request)
	// Bit 3 : Serial   Interrupt Request(INT 58h)    (1 = Request)
	// Bit 4 : Joypad   Interrupt Request(INT 60h)    (1 = Request)
	int IF = 0;
	bool VBlankRequest;
	bool LCDStatRequest;
	bool TimerRequest;
	bool SerialRequest;
	bool JoypadRequest;

	// FFFF: IE - Interrupt Enable Register
	// Bit 0 : VBlank   Interrupt Enable(INT 40h)  (1 = Enable)
	// Bit 1 : LCD STAT Interrupt Enable(INT 48h)  (1 = Enable)
	// Bit 2 : Timer    Interrupt Enable(INT 50h)  (1 = Enable)
	// Bit 3 : Serial   Interrupt Enable(INT 58h)  (1 = Enable)
	// Bit 4 : Joypad   Interrupt Enable(INT 60h)  (1 = Enable)
	int IE = 0;
	bool VBlankEnable;
	bool LCDStatEnable;
	bool TimerEnable;
	bool SerialEnable;
	bool JoypadEnable;
};

