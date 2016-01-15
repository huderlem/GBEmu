#pragma once

#include "CPUOpcodes.h"
#include "CPURegisters.h"
#include "IMMU.h"
#include "Interrupts.h"

#define TEST_API __declspec(dllexport)

class TEST_API CPU
{
public:
	CPU(IMMU *mmu, CPURegisters *registers, CPUOpcodes *opcodes);
	~CPU();
	int ExecuteNextInstruction(Interrupts *interrupts);
	void NotifyInterruptOccurred();
	bool InStopMode();
private:
	CPURegisters *registers;
	IMMU *mmu;
	CPUOpcodes *opcodes;

	bool haltState;
	bool stopState;
};

