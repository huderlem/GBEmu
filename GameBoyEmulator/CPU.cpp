#include "stdafx.h"
#include "CPU.h"

CPU::CPU(IMMU *mmu, CPURegisters *registers, CPUOpcodes *opcodes)
{
	CPU::registers = registers;
	CPU::mmu = mmu;
	CPU::opcodes = opcodes;
}

CPU::~CPU()
{
	//TODO: delete members ??
}

// Executes the next CPU instruction.
// Returns the number of CPU cycles that the instruction used.
int CPU::ExecuteNextInstruction()
{
	// Read the next opcode from the program counter.
	int opcode = mmu->ReadByte(registers->pc);
	registers->pc++;

	int cycles = opcodes->ExecuteInstruction(opcode, registers);
	return cycles;
}
