#include "stdafx.h"
#include "CPU.h"

CPU::CPU(IMMU *mmu, CPURegisters *registers, CPUOpcodes *opcodes)
{
	CPU::registers = registers;
	CPU::mmu = mmu;
	CPU::opcodes = opcodes;
	haltState = false;
}

CPU::~CPU()
{
	//TODO: delete members ??
}

// Executes the next CPU instruction.
// Returns the number of CPU cycles that the instruction used.
int CPU::ExecuteNextInstruction(Interrupts *interrupts)
{
	// In HALT state, just return some fake cycles to keep the rest of the system ticking.
	if (haltState)
	{
		return 4;
	}

	// Read the next opcode from the program counter.
	int opcode = mmu->ReadByte(registers->pc);
	registers->pc++;

	int cycles = opcodes->ExecuteInstruction(opcode, registers);

	// TODO: This is too hacky. And magic numbers are bad.
	if (cycles == -1)
	{
		haltState = true;
		// If interrupts are disabled, then the next instruction is skipped.
		if (!interrupts->InterruptsEnabled())
		{
			// TODO: This needs to be the length in bytes of the next instruction, not just an increment.
			registers->pc++;
		}

		return 4;
	}

	return cycles;
}

void CPU::NotifyInterruptOccurred()
{
	haltState = false;
}
