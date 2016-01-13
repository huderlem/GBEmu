#include "stdafx.h"
#include "Interrupts.h"


Interrupts::Interrupts()
{
	IME = true;
	VBlankEnable = 0;
	LCDStatEnable = 0;
	TimerEnable = 0;
	SerialEnable = 0;
	JoypadEnable = 0;
	VBlankRequest = 0;
	LCDStatRequest = 0;
	TimerRequest = 0;
	SerialRequest = 0;
	JoypadRequest = 0;
}


Interrupts::~Interrupts()
{
}

void Interrupts::DisableInterrupts()
{
	IME = false;
}

void Interrupts::EnableInterrupts()
{
	IME = true;
}

bool Interrupts::InterruptsEnabled()
{
	return IME;
}

void Interrupts::SetInterruptEnableRegister(int value)
{
	VBlankEnable  = (value & 0b00000001) > 0 ? 1 : 0;
	LCDStatEnable = (value & 0b00000010) > 0 ? 1 : 0;
	TimerEnable   = (value & 0b00000100) > 0 ? 1 : 0;
	SerialEnable  = (value & 0b00001000) > 0 ? 1 : 0;
	JoypadEnable  = (value & 0b00010000) > 0 ? 1 : 0;
}

int Interrupts::GetInterruptEnableRegister()
{
	return (JoypadEnable << 4) | (SerialEnable << 3) | (TimerEnable << 2) | (LCDStatEnable << 1) | VBlankEnable;
}

void Interrupts::SetInterruptRequestRegister(int value)
{
	VBlankRequest  = (value & 0b00000001) > 0 ? 1 : 0;
	LCDStatRequest = (value & 0b00000010) > 0 ? 1 : 0;
	TimerRequest   = (value & 0b00000100) > 0 ? 1 : 0;
	SerialRequest  = (value & 0b00001000) > 0 ? 1 : 0;
	JoypadRequest  = (value & 0b00010000) > 0 ? 1 : 0;
}

int Interrupts::GetInterruptRequestRegister()
{
	return (JoypadRequest << 4) | (SerialRequest << 3) | (TimerRequest << 2) | (LCDStatRequest << 1) | VBlankRequest;
}

void Interrupts::RequestVBlankInterrupt()
{
	VBlankRequest = true;
}

void Interrupts::RequestLCDStatInterrupt()
{
	LCDStatRequest = true;
}

void Interrupts::RequestTimerInterrupt()
{
	TimerRequest = true;
}

void Interrupts::RequestSerialInterrupt()
{
	SerialRequest = true;
}

void Interrupts::RequestJoypadInterrupt()
{
	JoypadRequest = true;
}

void Interrupts::ExecutePendingInterrupt(CPURegisters *registers, IMMU *mmu)
{
	if (!IME)
	{
		// All interrupts are disabled.
		return;
	}

	// Check for a requested interrupt in decreasing priority.
	if (VBlankRequest && VBlankEnable)
	{
		// Execute VBlank interrupt.
		VBlankRequest = false;
		IME = false;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = 0x0040;
	}
	else if (LCDStatRequest && LCDStatEnable)
	{
		// Execute LCD STAT interrupt.
		LCDStatRequest = false;
		IME = false;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = 0x0048;
	}
	else if (TimerRequest && TimerEnable)
	{
		// Execute Timer interrupt.
		TimerRequest = false;
		IME = false;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = 0x0050;
	}
	else if (SerialRequest && SerialEnable)
	{
		// Execute Timer interrupt.
		SerialRequest = false;
		IME = false;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = 0x0058;
	}
	else if (JoypadRequest && JoypadEnable)
	{
		// Execute Joypad interrupt.
		JoypadRequest = false;
		IME = false;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = 0x0060;
	}
}
