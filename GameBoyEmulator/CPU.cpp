#include "stdafx.h"
#include "CPU.h"

CPU::CPU(IMMU *mmu, CPURegisters *registers, Timer *timer, Interrupts *interrupts)
{
	CPU::registers = registers;
	CPU::mmu = mmu;
	CPU::timer = timer;
	CPU::interrupts = interrupts;
	haltState = false;
	stopState = false;
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
	if (haltState || stopState)
	{
		return 4;
	}

	// Read the next opcode from the program counter.
	int opcode = mmu->ReadByte(registers->pc);
	registers->pc++;

	int cycles = ExecuteInstruction(opcode, registers);

	// TODO: This is too hacky. And magic numbers are bad.
	if (cycles == -1)
	{
		// HALT instruction occurred.
		haltState = true;
		// If interrupts are disabled, then the next instruction is skipped.
		if (!interrupts->InterruptsEnabled())
		{
			// TODO: This needs to be the length in bytes of the next instruction, not just an increment.
			registers->pc++;
		}

		return 4;
	}
	else if (cycles == -2)
	{
		// STOP instruction occurred.
		stopState = true;
		return 4;
	}

	return cycles;
}

void CPU::NotifyInterruptOccurred()
{
	haltState = false;
	stopState = false;
}

bool CPU::InStopMode()
{
	return stopState;
}

void CPU::TickTimer(int cycles)
{
	bool timerInterruptRequested = timer->Tick(cycles, interrupts);
	if (timerInterruptRequested)
	{
		NotifyInterruptOccurred();
	}
}

// Executes the CPU instruction corresponding to the given opcode.
// Returns the number of CPU cycles used by the instruction.
int CPU::ExecuteInstruction(int opcode, CPURegisters *registers)
{
	Instruction_Func func = Instructions[opcode];
	int cycles = (this->*func)(registers);

	// Ensure lower-4 bits of register f are reset
	registers->f &= 0b11110000;

	return cycles;
}

// NOP
int CPU::op_00(CPURegisters *registers)
{
	return 4;
}

// ld bc, d16
int CPU::op_01(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->b = (word >> 8) & 0xff;
	registers->c = word & 0xff;
	return 12;
}

// ld (bc), a
int CPU::op_02(CPURegisters * registers)
{
	int address = (registers->b << 8) | registers->c;
	mmu->WriteByte(registers->a, address);
	return 8;
}

// inc bc
int CPU::op_03(CPURegisters * registers)
{
	int word = (registers->b << 8) | registers->c;
	word++;
	registers->b = (word >> 8) & 0xff;
	registers->c = word & 0xff;
	return 8;
}

// inc b
int CPU::op_04(CPURegisters * registers)
{
	registers->b = (registers->b + 1) & 0xff;
	if (registers->b == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->b & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec b
int CPU::op_05(CPURegisters * registers)
{
	if (registers->b == 0)
	{
		registers->b = 0xff;
	}
	else
	{
		registers->b--;
	}

	if (registers->b == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->b & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld b, d8
int CPU::op_06(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->b = byte;
	return 8;
}

// rlca
int CPU::op_07(CPURegisters * registers)
{
	int carry = (registers->a >> 7);
	registers->a = ((registers->a << 1) & 0xff) | carry;
	registers->f = carry << 4;
	return 4;
}

// ld (a16), sp
int CPU::op_08(CPURegisters * registers)
{
	int address = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	mmu->WriteWord(registers->sp, address);
	return 20;
}

// add hl, bc
int CPU::op_09(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int bc = (registers->b << 8) | registers->c;
	int result = hl + bc;
	if (result > 0xffff)
	{
		registers->f |= 0b00010000;  // set carry flag
	}
	else
	{
		registers->f &= 0b11101111;  // reset carry flag
	}

	if ((hl & 0xfff) + (bc & 0xfff) > 0xfff)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->h = (result >> 8) & 0xff;
	registers->l = result & 0xff;

	registers->f &= 0b10111111;  // reset subtract flag
	return 8;
}

// ld a, (bc)
int CPU::op_0A(CPURegisters * registers)
{
	int address = (registers->b << 8) | registers->c;
	registers->a = mmu->ReadByte(address);
	return 8;
}

// dec bc
int CPU::op_0B(CPURegisters * registers)
{
	int bc = (registers->b << 8) | registers->c;
	if (bc == 0)
	{
		registers->b = 0xff;
		registers->c = 0xff;
	}
	else
	{
		bc--;
		registers->b = (bc >> 8) & 0xff;
		registers->c = bc & 0xff;
	}

	return 8;
}

// inc c
int CPU::op_0C(CPURegisters * registers)
{
	registers->c = (registers->c + 1) & 0xff;
	if (registers->c == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->c & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec c
int CPU::op_0D(CPURegisters * registers)
{
	if (registers->c == 0)
	{
		registers->c = 0xff;
	}
	else
	{
		registers->c--;
	}

	if (registers->c == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->c & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld c, d8
int CPU::op_0E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->c = byte;
	return 8;
}

// rrca
int CPU::op_0F(CPURegisters * registers)
{
	int carry = (registers->a & 1);
	registers->a = (registers->a >> 1) | (carry << 7);
	registers->f = carry << 4;
	return 4;
}

// stop
int CPU::op_10(CPURegisters * registers)
{
	// TODO: figure how this actually works...
	return 4;
}

// ld de, d16
int CPU::op_11(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->d = (word >> 8) & 0xff;
	registers->e = word & 0xff;
	return 12;
}

// ld (de), a
int CPU::op_12(CPURegisters * registers)
{
	int address = (registers->d << 8) | registers->e;
	mmu->WriteByte(registers->a, address);
	return 8;
}

// inc de
int CPU::op_13(CPURegisters * registers)
{
	int de = (registers->d << 8) | registers->e;
	de++;
	registers->d = (de >> 8) & 0xff;
	registers->e = de & 0xff;
	return 8;
}

// inc d
int CPU::op_14(CPURegisters * registers)
{
	registers->d = (registers->d + 1) & 0xff;
	if (registers->d == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->d & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec d
int CPU::op_15(CPURegisters * registers)
{
	if (registers->d == 0)
	{
		registers->d = 0xff;
	}
	else
	{
		registers->d--;
	}

	if (registers->d == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->d & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld d, d8
int CPU::op_16(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->d = byte;
	return 8;
}

// rla
int CPU::op_17(CPURegisters * registers)
{
	int carry = (registers->f >> 4) & 1;
	if ((registers->a >> 7) == 1)
	{
		registers->f = 0b00010000;
	}
	else
	{
		registers->f = 0b00000000;
	}

	registers->a = ((registers->a << 1) | carry) & 0xff;
	return 4;
}

// jr r8
int CPU::op_18(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	if (offset > 0x7f)
	{
		registers->pc -= (0x100 - offset);
	}
	else
	{
		registers->pc += offset;
	}

	return 12;
}

// add hl, de
int CPU::op_19(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int de = (registers->d << 8) | registers->e;
	int result = hl + de;
	if (result > 0xffff)
	{
		registers->f |= 0b00010000;  // set carry flag
	}
	else
	{
		registers->f &= 0b11101111;  // reset carry flag
	}

	if ((hl & 0xfff) + (de & 0xfff) > 0xfff)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->h = (result >> 8) & 0xff;
	registers->l = result & 0xff;

	registers->f &= 0b10111111;  // reset subtract flag
	return 8;
}

// ld a, (de)
int CPU::op_1A(CPURegisters * registers)
{
	int address = (registers->d << 8) | registers->e;
	registers->a = mmu->ReadByte(address);
	return 8;
}

// dec de
int CPU::op_1B(CPURegisters * registers)
{
	int de = (registers->d << 8) | registers->e;
	if (de == 0)
	{
		registers->d = 0xff;
		registers->e = 0xff;
	}
	else
	{
		de--;
		registers->d = (de >> 8) & 0xff;
		registers->e = de & 0xff;
	}

	return 8;
}

// inc e
int CPU::op_1C(CPURegisters * registers)
{
	registers->e = (registers->e + 1) & 0xff;
	if (registers->e == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->e & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec e
int CPU::op_1D(CPURegisters * registers)
{
	if (registers->e == 0)
	{
		registers->e = 0xff;
	}
	else
	{
		registers->e--;
	}

	if (registers->e == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->e & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld e, d8
int CPU::op_1E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->e = byte;
	return 8;
}

// rra
int CPU::op_1F(CPURegisters * registers)
{
	int carry = (registers->f >> 4) & 1;
	int newCarry = (registers->a & 1);
	registers->a = (registers->a >> 1) | (carry << 7);
	registers->f = (newCarry << 4);
	return 4;
}

// jr nz, r8
int CPU::op_20(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	if ((registers->f & 0b10000000) == 0)
	{
		if (offset > 0x7f)
		{
			registers->pc -= (0x100 - offset);
		}
		else
		{
			registers->pc += offset;
		}

		return 12;
	}
	else
	{
		return 8;
	}
}

// ld hl, d16
int CPU::op_21(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->h = (word >> 8) & 0xff;
	registers->l = word & 0xff;
	return 12;
}

// ld (hl+), a
int CPU::op_22(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->a, hl);
	hl++;
	registers->h = (hl >> 8) & 0xff;
	registers->l = (hl & 0xff);
	return 8;
}

// inc hl
int CPU::op_23(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	hl++;
	registers->h = (hl >> 8) & 0xff;
	registers->l = hl & 0xff;
	return 8;
}

// inc h
int CPU::op_24(CPURegisters * registers)
{
	registers->h = (registers->h + 1) & 0xff;
	if (registers->h == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->h & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec h
int CPU::op_25(CPURegisters * registers)
{
	if (registers->h == 0)
	{
		registers->h = 0xff;
	}
	else
	{
		registers->h--;
	}

	if (registers->h == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->h & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld h, d8
int CPU::op_26(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->h = byte;
	return 8;
}

// daa
// See table here: http://www.z80.info/z80syntx.htm#DAA
int CPU::op_27(CPURegisters * registers)
{
	int correction = 0;
	bool subtractFlag = (registers->f & 0b01000000) > 0;
	bool halfCarryFlag = (registers->f & 0b00100000) > 0;
	bool carryFlag = (registers->f & 0b00010000) > 0;
	if (subtractFlag)
	{
		if (halfCarryFlag)
		{
			correction -= 6;
		}

		if (carryFlag)
		{
			correction -= 0x60;
		}
	}
	else
	{
		if (halfCarryFlag || ((registers->a & 0xf) > 0x09))
		{
			correction += 6;
		}

		if (carryFlag || (registers->a >= 0x9a))
		{
			correction += 0x60;
		}
	}

	registers->a = (registers->a + correction) & 0xff;

	// set carry flag appropriately
	if (correction >= 0x60 || correction <= -0x60)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// reset half carry
	registers->f &= 0b11011111;

	// set zero flag appropriately
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// jr z, r8
int CPU::op_28(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	if ((registers->f & 0b10000000) > 0)
	{
		if (offset > 0x7f)
		{
			registers->pc -= (0x100 - offset);
		}
		else
		{
			registers->pc += offset;
		}

		return 12;
	}
	else
	{
		return 8;
	}
}

// add hl, hl
int CPU::op_29(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int hlCopy = hl;
	int result = hl + hlCopy;
	if (result > 0xffff)
	{
		registers->f |= 0b00010000;  // set carry flag
	}
	else
	{
		registers->f &= 0b11101111;  // reset carry flag
	}

	if ((hl & 0xfff) + (hlCopy & 0xfff) > 0xfff)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->h = (result >> 8) & 0xff;
	registers->l = result & 0xff;

	registers->f &= 0b10111111;  // reset subtract flag
	return 8;
}

// ld a, (hl+)
int CPU::op_2A(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	registers->a = value;
	hl++;
	registers->h = (hl >> 8) & 0xff;
	registers->l = hl & 0xff;
	return 8;
}

// dec hl
int CPU::op_2B(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	if (hl == 0)
	{
		registers->h = 0xff;
		registers->l = 0xff;
	}
	else
	{
		hl--;
		registers->h = (hl >> 8) & 0xff;
		registers->l = hl & 0xff;
	}

	return 8;
}

// inc l
int CPU::op_2C(CPURegisters * registers)
{
	registers->l = (registers->l + 1) & 0xff;
	if (registers->l == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->l & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec l
int CPU::op_2D(CPURegisters * registers)
{
	if (registers->l == 0)
	{
		registers->l = 0xff;
	}
	else
	{
		registers->l--;
	}

	if (registers->l == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->l & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld l, d8
int CPU::op_2E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->l = byte;
	return 8;
}

// cpl
int CPU::op_2F(CPURegisters * registers)
{
	registers->a = (~registers->a & 0xff);
	registers->f |= 0b01100000;
	return 4;
}

// jr nc, r8
int CPU::op_30(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	if ((registers->f & 0b00010000) == 0)
	{
		if (offset > 0x7f)
		{
			registers->pc -= (0x100 - offset);
		}
		else
		{
			registers->pc += offset;
		}

		return 12;
	}
	else
	{
		return 8;
	}
}

// ld sp, d16
int CPU::op_31(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->sp = word;
	return 12;
}

// ld (hl-), a
int CPU::op_32(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->a, hl);
	hl--;
	registers->h = (hl >> 8) & 0xff;
	registers->l = (hl & 0xff);
	return 8;
}

// inc sp
int CPU::op_33(CPURegisters * registers)
{
	registers->sp = (registers->sp + 1) & 0xffff;
	return 8;
}

// inc (hl)
int CPU::op_34(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	value = (value + 1) & 0xff;
	if (value == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((value & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag

	TickTimer(4);
	mmu->WriteByte(value, hl);
	return 8;
}

// dec (hl)
int CPU::op_35(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	if (value == 0)
	{
		value = 0xff;
	}
	else
	{
		value--;
	}

	if (value == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((value & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag

	TickTimer(4);
	mmu->WriteByte(value, hl);
	return 8;
}

// ld (hl), d8
int CPU::op_36(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	TickTimer(4);
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(byte, hl);
	return 8;
}

// scf
int CPU::op_37(CPURegisters * registers)
{
	registers->f &= 0b10011111;
	registers->f |= 0b00010000;
	return 4;
}

// jr c, r8
int CPU::op_38(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	if ((registers->f & 0b00010000) > 0)
	{
		if (offset > 0x7f)
		{
			registers->pc -= (0x100 - offset);
		}
		else
		{
			registers->pc += offset;
		}

		return 12;
	}
	else
	{
		return 8;
	}
}

// add hl, sp
int CPU::op_39(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int result = hl + registers->sp;
	if (result > 0xffff)
	{
		registers->f |= 0b00010000;  // set carry flag
	}
	else
	{
		registers->f &= 0b11101111;  // reset carry flag
	}

	if ((hl & 0xfff) + (registers->sp & 0xfff) > 0xfff)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->h = (result >> 8) & 0xff;
	registers->l = result & 0xff;

	registers->f &= 0b10111111;  // reset subtract flag
	return 8;
}

// ld a, (hl-)
int CPU::op_3A(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	registers->a = value;
	hl--;
	registers->h = (hl >> 8) & 0xff;
	registers->l = hl & 0xff;
	return 8;
}

// dec sp
int CPU::op_3B(CPURegisters * registers)
{
	if (registers->sp == 0)
	{
		registers->sp = 0xffff;
	}
	else
	{
		registers->sp = (registers->sp - 1) & 0xffff;
	}

	return 8;
}

// inc a
int CPU::op_3C(CPURegisters * registers)
{
	registers->a = (registers->a + 1) & 0xff;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->a & 0xf) == 0)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f &= 0b10111111;  // clear subtract flag
	return 4;
}

// dec a
int CPU::op_3D(CPURegisters * registers)
{
	if (registers->a == 0)
	{
		registers->a = 0xff;
	}
	else
	{
		registers->a--;
	}

	if (registers->a == 0)
	{
		registers->f |= 0b10000000;  // set zero flag
	}
	else
	{
		registers->f &= 0b01111111;  // reset zero flag
	}

	if ((registers->a & 0x0f) == 0x0f)
	{
		registers->f |= 0b00100000;  // set half carry flag
	}
	else
	{
		registers->f &= 0b11011111;  // reset half carry flag
	}

	registers->f |= 0b01000000;  // set subtract flag
	return 4;
}

// ld a, d8
int CPU::op_3E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->a = byte;
	return 8;
}

// ccf
int CPU::op_3F(CPURegisters * registers)
{
	registers->f ^= 0b00010000;
	registers->f = (registers->f & 0b10010000) & 0xff;
	return 4;
}

// ld b, b
int CPU::op_40(CPURegisters * registers)
{
	return 4;
}

// ld b, c
int CPU::op_41(CPURegisters * registers)
{
	registers->b = registers->c;
	return 4;
}

// ld b, d
int CPU::op_42(CPURegisters * registers)
{
	registers->b = registers->d;
	return 4;
}

// ld b, e
int CPU::op_43(CPURegisters * registers)
{
	registers->b = registers->e;
	return 4;
}

// ld b, h
int CPU::op_44(CPURegisters * registers)
{
	registers->b = registers->h;
	return 4;
}

// ld b, l
int CPU::op_45(CPURegisters * registers)
{
	registers->b = registers->l;
	return 4;
}

// ld b, (hl)
int CPU::op_46(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->b = mmu->ReadByte(hl);
	return 8;
}

// ld b, a
int CPU::op_47(CPURegisters * registers)
{
	registers->b = registers->a;
	return 4;
}

// ld c, b
int CPU::op_48(CPURegisters * registers)
{
	registers->c = registers->b;
	return 4;
}

// ld c, c
int CPU::op_49(CPURegisters * registers)
{
	return 4;
}

// ld c, d
int CPU::op_4A(CPURegisters * registers)
{
	registers->c = registers->d;
	return 4;
}

// ld c, e
int CPU::op_4B(CPURegisters * registers)
{
	registers->c = registers->e;
	return 4;
}

// ld c, h
int CPU::op_4C(CPURegisters * registers)
{
	registers->c = registers->h;
	return 4;
}

// ld c, l
int CPU::op_4D(CPURegisters * registers)
{
	registers->c = registers->l;
	return 4;
}

// ld c, (hl)
int CPU::op_4E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->c = mmu->ReadByte(hl);
	return 8;
}

// ld c, a
int CPU::op_4F(CPURegisters * registers)
{
	registers->c = registers->a;
	return 4;
}

// ld d, b
int CPU::op_50(CPURegisters * registers)
{
	registers->d = registers->b;
	return 4;
}

// ld d, c
int CPU::op_51(CPURegisters * registers)
{
	registers->d = registers->c;
	return 4;
}

// ld d, d
int CPU::op_52(CPURegisters * registers)
{
	return 4;
}

// ld d, e
int CPU::op_53(CPURegisters * registers)
{
	registers->d = registers->e;
	return 4;
}

// ld d, h
int CPU::op_54(CPURegisters * registers)
{
	registers->d = registers->h;
	return 4;
}

// ld d, l
int CPU::op_55(CPURegisters * registers)
{
	registers->d = registers->l;
	return 4;
}

// ld d, (hl)
int CPU::op_56(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->d = mmu->ReadByte(hl);
	return 8;
}

// ld d, a
int CPU::op_57(CPURegisters * registers)
{
	registers->d = registers->a;
	return 4;
}

// ld e, b
int CPU::op_58(CPURegisters * registers)
{
	registers->e = registers->b;
	return 4;
}

// ld e, c
int CPU::op_59(CPURegisters * registers)
{
	registers->e = registers->c;
	return 4;
}

// ld e, d
int CPU::op_5A(CPURegisters * registers)
{
	registers->e = registers->d;
	return 4;
}

// ld e, e
int CPU::op_5B(CPURegisters * registers)
{
	return 4;
}

// ld e, h
int CPU::op_5C(CPURegisters * registers)
{
	registers->e = registers->h;
	return 4;
}

// ld e, l
int CPU::op_5D(CPURegisters * registers)
{
	registers->e = registers->l;
	return 4;
}

// ld e, (hl)
int CPU::op_5E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->e = mmu->ReadByte(hl);
	return 8;
}

// ld e, a
int CPU::op_5F(CPURegisters * registers)
{
	registers->e = registers->a;
	return 4;
}

// ld h, b
int CPU::op_60(CPURegisters * registers)
{
	registers->h = registers->b;
	return 4;
}

// ld h, c
int CPU::op_61(CPURegisters * registers)
{
	registers->h = registers->c;
	return 4;
}

// ld h, d
int CPU::op_62(CPURegisters * registers)
{
	registers->h = registers->d;
	return 4;
}

// ld h, e
int CPU::op_63(CPURegisters * registers)
{
	registers->h = registers->e;
	return 4;
}

// ld h, h
int CPU::op_64(CPURegisters * registers)
{
	return 4;
}

// ld h, l
int CPU::op_65(CPURegisters * registers)
{
	registers->h = registers->l;
	return 4;
}

// ld h, (hl)
int CPU::op_66(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->h = mmu->ReadByte(hl);
	return 8;
}

// ld h, a
int CPU::op_67(CPURegisters * registers)
{
	registers->h = registers->a;
	return 4;
}

// ld l, b
int CPU::op_68(CPURegisters * registers)
{
	registers->l = registers->b;
	return 4;
}

// ld l, c
int CPU::op_69(CPURegisters * registers)
{
	registers->l = registers->c;
	return 4;
}

// ld l, d
int CPU::op_6A(CPURegisters * registers)
{
	registers->l = registers->d;
	return 4;
}

// ld l, e
int CPU::op_6B(CPURegisters * registers)
{
	registers->l = registers->e;
	return 4;
}

// ld l, h
int CPU::op_6C(CPURegisters * registers)
{
	registers->l = registers->h;
	return 4;
}

// ld l, l
int CPU::op_6D(CPURegisters * registers)
{
	return 4;
}

// ld l, (hl)
int CPU::op_6E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->l = mmu->ReadByte(hl);
	return 8;
}

// ld l, a
int CPU::op_6F(CPURegisters * registers)
{
	registers->l = registers->a;
	return 4;
}

// ld (hl), b
int CPU::op_70(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->b, hl);
	return 8;
}

// ld (hl), c
int CPU::op_71(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->c, hl);
	return 8;
}

// ld (hl), d
int CPU::op_72(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->d, hl);
	return 8;
}

// ld (hl), e
int CPU::op_73(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->e, hl);
	return 8;
}

// ld (hl), h
int CPU::op_74(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->h, hl);
	return 8;
}

// ld (hl), l
int CPU::op_75(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->l, hl);
	return 8;
}

// halt
int CPU::op_76(CPURegisters * registers)
{ // TODO: get rid of this magic number
	return -1;
}

// ld (hl), a
int CPU::op_77(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->a, hl);
	return 8;
}

// ld a, b
int CPU::op_78(CPURegisters * registers)
{
	registers->a = registers->b;
	return 4;
}

// ld a, c
int CPU::op_79(CPURegisters * registers)
{
	registers->a = registers->c;
	return 4;
}

// ld a, d
int CPU::op_7A(CPURegisters * registers)
{
	registers->a = registers->d;
	return 4;
}

// ld a, e
int CPU::op_7B(CPURegisters * registers)
{
	registers->a = registers->e;
	return 4;
}

// ld a, h
int CPU::op_7C(CPURegisters * registers)
{
	registers->a = registers->h;
	return 4;
}

// ld a, l
int CPU::op_7D(CPURegisters * registers)
{
	registers->a = registers->l;
	return 4;
}

// ld a, (hl)
int CPU::op_7E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->a = mmu->ReadByte(hl);
	return 8;
}

// ld a, a
int CPU::op_7F(CPURegisters * registers)
{
	return 4;
}

// add a, b	
int CPU::op_80(CPURegisters * registers)
{
	int result = registers->a + registers->b;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->b & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// add a, c
int CPU::op_81(CPURegisters * registers)
{
	int result = registers->a + registers->c;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->c & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// add a, d
int CPU::op_82(CPURegisters * registers)
{
	int result = registers->a + registers->d;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->d & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// add a, e
int CPU::op_83(CPURegisters * registers)
{
	int result = registers->a + registers->e;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->e & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// add a, h
int CPU::op_84(CPURegisters * registers)
{
	int result = registers->a + registers->h;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->h & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// add a, l
int CPU::op_85(CPURegisters * registers)
{
	int result = registers->a + registers->l;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->l & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// add a, (hl)
int CPU::op_86(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	int result = registers->a + value;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (value & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 8;
}

// add a, a
int CPU::op_87(CPURegisters * registers)
{
	int result = registers->a + registers->a;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->a & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, b
int CPU::op_88(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->b + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->b & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, c
int CPU::op_89(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->c + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->c & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, d
int CPU::op_8A(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->d + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->d & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, e
int CPU::op_8B(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->e + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->e & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, h
int CPU::op_8C(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->h + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->h & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, l
int CPU::op_8D(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->l + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->l & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// adc a, (hl)
int CPU::op_8E(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	int result = registers->a + value + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (value & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 8;
}

// adc a, a
int CPU::op_8F(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + registers->a + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (registers->a & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 4;
}

// sub b
int CPU::op_90(CPURegisters * registers)
{
	int result = registers->a - registers->b;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->b & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sub c
int CPU::op_91(CPURegisters * registers)
{
	int result = registers->a - registers->c;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->c & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sub d
int CPU::op_92(CPURegisters * registers)
{
	int result = registers->a - registers->d;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->d & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sub e
int CPU::op_93(CPURegisters * registers)
{
	int result = registers->a - registers->e;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->e & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sub h
int CPU::op_94(CPURegisters * registers)
{
	int result = registers->a - registers->h;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->h & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sub l
int CPU::op_95(CPURegisters * registers)
{
	int result = registers->a - registers->l;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->l & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sub (hl)
int CPU::op_96(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	int result = registers->a - value;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (value & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 8;
}

// sub a
int CPU::op_97(CPURegisters * registers)
{
	int result = registers->a - registers->a;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->a & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc b
int CPU::op_98(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->b - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->b & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc c
int CPU::op_99(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->c - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->c & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc d
int CPU::op_9A(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->d - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->d & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc e
int CPU::op_9B(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->e - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->e & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc h
int CPU::op_9C(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->h - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->h & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc l
int CPU::op_9D(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->l - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->l & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// sbc (hl)
int CPU::op_9E(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	int result = registers->a - value - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (value & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 8;
}

// sbc a
int CPU::op_9F(CPURegisters * registers)
{
	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - registers->a - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->a & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 4;
}

// and b
int CPU::op_A0(CPURegisters * registers)
{
	registers->a = registers->a & registers->b;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// and c
int CPU::op_A1(CPURegisters * registers)
{
	registers->a = registers->a & registers->c;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// and d
int CPU::op_A2(CPURegisters * registers)
{
	registers->a = registers->a & registers->d;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// and e
int CPU::op_A3(CPURegisters * registers)
{
	registers->a = registers->a & registers->e;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// and h
int CPU::op_A4(CPURegisters * registers)
{
	registers->a = registers->a & registers->h;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// and l
int CPU::op_A5(CPURegisters * registers)
{
	registers->a = registers->a & registers->l;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// and (hl)
int CPU::op_A6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	registers->a = registers->a & value;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 8;
}

// and a
int CPU::op_A7(CPURegisters * registers)
{
	registers->a = registers->a & registers->a;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor b
int CPU::op_A8(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->b;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor c
int CPU::op_A9(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->c;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor d
int CPU::op_AA(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->d;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor e
int CPU::op_AB(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->e;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor h
int CPU::op_AC(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->h;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor l
int CPU::op_AD(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->l;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// xor (hl)
int CPU::op_AE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	registers->a = registers->a ^ value;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 8;
}

// xor a
int CPU::op_AF(CPURegisters * registers)
{
	registers->a = registers->a ^ registers->a;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or b
int CPU::op_B0(CPURegisters * registers)
{
	registers->a = registers->a | registers->b;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or c
int CPU::op_B1(CPURegisters * registers)
{
	registers->a = registers->a | registers->c;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or d
int CPU::op_B2(CPURegisters * registers)
{
	registers->a = registers->a | registers->d;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or e
int CPU::op_B3(CPURegisters * registers)
{
	registers->a = registers->a | registers->e;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or h
int CPU::op_B4(CPURegisters * registers)
{
	registers->a = registers->a | registers->h;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or l
int CPU::op_B5(CPURegisters * registers)
{
	registers->a = registers->a | registers->l;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// or (hl)
int CPU::op_B6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	registers->a = registers->a | value;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 8;
}

// or a
int CPU::op_B7(CPURegisters * registers)
{
	registers->a = registers->a | registers->a;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 4;
}

// cp b
int CPU::op_B8(CPURegisters * registers)
{
	int result = registers->a - registers->b;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->b & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// cp c
int CPU::op_B9(CPURegisters * registers)
{
	int result = registers->a - registers->c;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->c & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// cp d
int CPU::op_BA(CPURegisters * registers)
{
	int result = registers->a - registers->d;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->d & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// cp e
int CPU::op_BB(CPURegisters * registers)
{
	int result = registers->a - registers->e;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->e & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// cp h
int CPU::op_BC(CPURegisters * registers)
{
	int result = registers->a - registers->h;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->h & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// cp l
int CPU::op_BD(CPURegisters * registers)
{
	int result = registers->a - registers->l;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->l & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// cp (hl)
int CPU::op_BE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	int value = mmu->ReadByte(hl);
	int result = registers->a - value;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (value & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 8;
}

// cp a
int CPU::op_BF(CPURegisters * registers)
{
	int result = registers->a - registers->a;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (registers->a & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 4;
}

// ret nz
int CPU::op_C0(CPURegisters * registers)
{
	if ((registers->f & 0b10000000) == 0)
	{
		registers->pc = mmu->ReadWord(registers->sp);
		registers->sp += 2;
		return 20;
	}
	else
	{
		return 8;
	}
}

// pop bc
int CPU::op_C1(CPURegisters * registers)
{
	registers->c = mmu->ReadByte(registers->sp);
	registers->b = mmu->ReadByte(registers->sp + 1);
	registers->sp += 2;
	return 12;
}

// jp nz, a16
int CPU::op_C2(CPURegisters * registers)
{
	if ((registers->f & 0b10000000) == 0)
	{
		registers->pc = mmu->ReadWord(registers->pc);
		return 16;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// jp a16
int CPU::op_C3(CPURegisters * registers)
{
	registers->pc = mmu->ReadWord(registers->pc);
	return 16;
}

// call nz, a16
int CPU::op_C4(CPURegisters * registers)
{
	if ((registers->f & 0b10000000) == 0)
	{
		int address = mmu->ReadWord(registers->pc);
		registers->pc += 2;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = address;
		return 24;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// push bc
int CPU::op_C5(CPURegisters * registers)
{
	int bc = (registers->b << 8) | registers->c;
	registers->sp -= 2;
	mmu->WriteWord(bc, registers->sp);
	return 16;
}

// add a, d8
int CPU::op_C6(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int result = registers->a + value;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (value & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 8;
}

// rst 00H
int CPU::op_C7(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0000;
	return 16;
}

// ret z
int CPU::op_C8(CPURegisters * registers)
{
	if ((registers->f & 0b10000000) > 0)
	{
		registers->pc = mmu->ReadWord(registers->sp);
		registers->sp += 2;
		return 20;
	}
	else
	{
		return 8;
	}
}

// ret
int CPU::op_C9(CPURegisters * registers)
{
	registers->pc = mmu->ReadWord(registers->sp);
	registers->sp += 2;
	return 16;
}

// jp z, a16
int CPU::op_CA(CPURegisters * registers)
{
	if ((registers->f & 0b10000000) > 0)
	{
		registers->pc = mmu->ReadWord(registers->pc);
		return 16;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// CB XX
int CPU::op_CB(CPURegisters * registers)
{
	int opcode = mmu->ReadByte(registers->pc);
	registers->pc++;

	Instruction_Func CB_func = CB_Instructions[opcode];
	int cycles = (this->*CB_func)(registers);
	return cycles;
}

// call z, a16
int CPU::op_CC(CPURegisters * registers)
{
	if ((registers->f & 0b10000000) > 0)
	{
		int address = mmu->ReadWord(registers->pc);
		registers->pc += 2;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = address;
		return 24;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// call a16
int CPU::op_CD(CPURegisters * registers)
{
	int address = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = address;
	return 24;
}

// adc a, d8
int CPU::op_CE(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a + value + carry;
	// carry flag
	if (result > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) + (value & 0xf) + carry > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// reset subtract flag
	registers->f &= 0b10111111;

	registers->a = result;
	return 8;
}

// rst 08H
int CPU::op_CF(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0008;
	return 16;
}

// ret nc
int CPU::op_D0(CPURegisters * registers)
{
	if ((registers->f & 0b00010000) == 0)
	{
		registers->pc = mmu->ReadWord(registers->sp);
		registers->sp += 2;
		return 20;
	}
	else
	{
		return 8;
	}
}

// pop de
int CPU::op_D1(CPURegisters * registers)
{
	registers->e = mmu->ReadByte(registers->sp);
	registers->d = mmu->ReadByte(registers->sp + 1);
	registers->sp += 2;
	return 12;
}

// jp nc, a16
int CPU::op_D2(CPURegisters * registers)
{
	if ((registers->f & 0b00010000) == 0)
	{
		registers->pc = mmu->ReadWord(registers->pc);
		return 16;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// call nc, a16
int CPU::op_D4(CPURegisters * registers)
{
	if ((registers->f & 0b00010000) == 0)
	{
		int address = mmu->ReadWord(registers->pc);
		registers->pc += 2;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = address;
		return 24;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// push de
int CPU::op_D5(CPURegisters * registers)
{
	int de = (registers->d << 8) | registers->e;
	registers->sp -= 2;
	mmu->WriteWord(de, registers->sp);
	return 16;
}

// sub d8
int CPU::op_D6(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int result = registers->a - value;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (value & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 8;
}

// rst 10H
int CPU::op_D7(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0010;
	return 16;
}

// ret c
int CPU::op_D8(CPURegisters * registers)
{
	if ((registers->f & 0b00010000) > 0)
	{
		registers->pc = mmu->ReadWord(registers->sp);
		registers->sp += 2;
		return 20;
	}
	else
	{
		return 8;
	}
}

// reti
int CPU::op_D9(CPURegisters * registers)
{
	registers->pc = mmu->ReadWord(registers->sp);
	registers->sp += 2;
	interrupts->EnableInterrupts();
	return 16;
}

// jp c, a16
int CPU::op_DA(CPURegisters * registers)
{
	if ((registers->f & 0b00010000) > 0)
	{
		registers->pc = mmu->ReadWord(registers->pc);
		return 16;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// call c, a16
int CPU::op_DC(CPURegisters * registers)
{
	if ((registers->f & 0b00010000) > 0)
	{
		int address = mmu->ReadWord(registers->pc);
		registers->pc += 2;
		registers->sp -= 2;
		mmu->WriteWord(registers->pc, registers->sp);
		registers->pc = address;
		return 24;
	}
	else
	{
		registers->pc += 2;
		return 12;
	}
}

// sbc a, d8
int CPU::op_DE(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int carry = ((registers->f & 0b00010000) >> 4);
	int result = registers->a - value - carry;
	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (value & 0xf) - carry < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xff;

	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	registers->a = result;
	return 8;
}

// rst 18H
int CPU::op_DF(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0018;
	return 16;
}

// ldh (a8), a
int CPU::op_E0(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	TickTimer(4);
	mmu->WriteByte(registers->a, 0xff00 + offset);
	return 8;
}

// pop hl
int CPU::op_E1(CPURegisters * registers)
{
	registers->l = mmu->ReadByte(registers->sp);
	registers->h = mmu->ReadByte(registers->sp + 1);
	registers->sp += 2;
	return 12;
}

// ld (c), a
int CPU::op_E2(CPURegisters * registers)
{
	mmu->WriteByte(registers->a, 0xff00 + registers->c);
	return 8;
}

// push hl
int CPU::op_E5(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->sp -= 2;
	mmu->WriteWord(hl, registers->sp);
	return 16;
}

// and d8
int CPU::op_E6(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	registers->a = registers->a & value;
	// reset subtract and carry flags
	registers->f &= 0b10101111;
	// set half carry flag
	registers->f |= 0b00100000;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 8;
}

// rst 20H
int CPU::op_E7(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0020;
	return 16;
}

// add sp, r8
int CPU::op_E8(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int result;
	if (value > 0x7f)
	{
		// immediate value is treated as negative integer
		result = registers->sp - (0x100 - value);
	}
	else
	{
		result = registers->sp + value;
	}

	// carry flag
	if (((registers->sp & 0xff) + value) > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->sp & 0xf) + (value & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xffff;

	// reset subtract and zero flags
	registers->f &= 0b00111111;

	registers->sp = result;
	return 16;
}

// jp hl
int CPU::op_E9(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->pc = hl;
	return 4;
}

// ld (a16), a
int CPU::op_EA(CPURegisters * registers)
{
	int address = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	TickTimer(8);
	mmu->WriteByte(registers->a, address);
	return 8;
}

// xor d8
int CPU::op_EE(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	registers->a = registers->a ^ value;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 8;
}

// rst 28H
int CPU::op_EF(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0028;
	return 16;
}

// ldh a, (a8)
int CPU::op_F0(CPURegisters * registers)
{
	int offset = mmu->ReadByte(registers->pc);
	registers->pc++;
	TickTimer(4);
	registers->a = mmu->ReadByte(0xFF00 + offset);
	return 8;
}

// pop af
int CPU::op_F1(CPURegisters * registers)
{
	registers->f = mmu->ReadByte(registers->sp);
	registers->a = mmu->ReadByte(registers->sp + 1);
	registers->sp += 2;
	return 12;
}

// ld a, (c)
int CPU::op_F2(CPURegisters * registers)
{
	registers->a = mmu->ReadByte(0xff00 + registers->c);
	return 8;
}

// di
int CPU::op_F3(CPURegisters * registers)
{
	interrupts->DisableInterrupts();
	return 4;
}

// push af
int CPU::op_F5(CPURegisters * registers)
{
	int af = (registers->a << 8) | registers->f;
	registers->sp -= 2;
	mmu->WriteWord(af, registers->sp);
	return 16;
}

// or d8
int CPU::op_F6(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	registers->a = registers->a | value;
	// reset subtract, half carry, and carry flags
	registers->f &= 0b10001111;
	if (registers->a == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	return 8;
}

// rst 30H
int CPU::op_F7(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0030;
	return 16;
}

// ld hl, sp + r8
int CPU::op_F8(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int result;
	if (value > 0x7f)
	{
		// immediate value is treated as negative integer
		result = registers->sp - (0x100 - value);
	}
	else
	{
		result = registers->sp + value;
	}

	// carry flag
	if (((registers->sp & 0xff) + value) > 0xff)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->sp & 0xf) + (value & 0xf) > 0xf)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	result = result & 0xffff;
	registers->h = (result >> 8);
	registers->l = (result & 0xff);

	// reset zero and subtrat flags
	registers->f &= 0b00111111;

	return 12;
}

// ld sp, hl
int CPU::op_F9(CPURegisters * registers)
{
	registers->sp = ((registers->h << 8) | registers->l);
	return 8;
}

// ld a, (a16)
int CPU::op_FA(CPURegisters * registers)
{
	int address = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	TickTimer(8);
	registers->a = mmu->ReadByte(address);
	return 8;
}

// ei
int CPU::op_FB(CPURegisters * registers)
{
	interrupts->EnableInterrupts();
	return 4;
}

// cp d8
int CPU::op_FE(CPURegisters * registers)
{
	int value = mmu->ReadByte(registers->pc);
	registers->pc++;

	int result = registers->a - value;
	// zero flag
	if (result == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	// carry flag
	if (result < 0)
	{
		registers->f |= 0b00010000;
	}
	else
	{
		registers->f &= 0b11101111;
	}

	// half carry flag
	if ((registers->a & 0xf) - (value & 0xf) < 0)
	{
		registers->f |= 0b00100000;
	}
	else
	{
		registers->f &= 0b11011111;
	}

	// set subtract flag
	registers->f |= 0b01000000;

	return 8;
}

// rst 38H
int CPU::op_FF(CPURegisters * registers)
{
	registers->sp -= 2;
	mmu->WriteWord(registers->pc, registers->sp);
	registers->pc = 0x0038;
	return 16;
}

// Helper for opcodes CB 00 through CB 07.
// Performs RLC on the given value.
void CPU::op_CB_RLC(CPURegisters *registers, int *value)
{
	int carry = (*value >> 7);
	*value = ((*value << 1) & 0xff) | carry;
	registers->f = carry << 4;
	if (*value == 0)
	{
		registers->f |= 0b10000000;
	}
}

// rlc b
int CPU::op_CB00(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->b);
	return 8;
}

// rlc c
int CPU::op_CB01(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->c);
	return 8;
}

// rlc d
int CPU::op_CB02(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->d);
	return 8;
}

// rlc e
int CPU::op_CB03(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->e);
	return 8;
}

// rlc h
int CPU::op_CB04(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->h);
	return 8;
}

// rlc l
int CPU::op_CB05(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->l);
	return 8;
}

// rlc (hl)
int CPU::op_CB06(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RLC(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// rlc a
int CPU::op_CB07(CPURegisters * registers)
{
	op_CB_RLC(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 08 through CB 0F.
// Performs RRC on the given value.
void CPU::op_CB_RRC(CPURegisters *registers, int *value)
{
	int carry = (*value & 1);
	*value = (*value >> 1) | (carry << 7);
	registers->f = carry << 4;
	if (*value == 0)
	{
		registers->f |= 0b10000000;
	}
}

// rrc b
int CPU::op_CB08(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->b);
	return 8;
}

// rrc c
int CPU::op_CB09(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->c);
	return 8;
}

// rrc d
int CPU::op_CB0A(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->d);
	return 8;
}

// rrc e
int CPU::op_CB0B(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->e);
	return 8;
}

// rrc h
int CPU::op_CB0C(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->h);
	return 8;
}

// rrc l
int CPU::op_CB0D(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->l);
	return 8;
}

// rrc (hl)
int CPU::op_CB0E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RRC(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// rrc a
int CPU::op_CB0F(CPURegisters * registers)
{
	op_CB_RRC(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 10 through CB 17.
// Performs RL on the given value.
void CPU::op_CB_RL(CPURegisters *registers, int *value)
{
	int carry = (registers->f >> 4) & 1;
	if ((*value >> 7) == 1)
	{
		registers->f = 0b00010000;
	}
	else
	{
		registers->f = 0b00000000;
	}

	*value = ((*value << 1) | carry) & 0xff;
	if (*value == 0)
	{
		registers->f |= 0b10000000;
	}
}

// rl b
int CPU::op_CB10(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->b);
	return 8;
}

// rl c
int CPU::op_CB11(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->c);
	return 8;
}

// rl d
int CPU::op_CB12(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->d);
	return 8;
}

// rl e
int CPU::op_CB13(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->e);
	return 8;
}

// rl h
int CPU::op_CB14(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->h);
	return 8;
}

// rl l
int CPU::op_CB15(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->l);
	return 8;
}

// rl (hl)
int CPU::op_CB16(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RL(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// rl a
int CPU::op_CB17(CPURegisters * registers)
{
	op_CB_RL(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 18 through CB 1F.
// Performs RR on the given value.
void CPU::op_CB_RR(CPURegisters *registers, int *value)
{
	int carry = (registers->f >> 4) & 1;
	int newCarry = (*value & 1);
	*value = (*value >> 1) | (carry << 7);
	registers->f = (newCarry << 4);
	if (*value == 0)
	{
		registers->f |= 0b10000000;
	}
}

// rr b
int CPU::op_CB18(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->b);
	return 8;
}

// rr c
int CPU::op_CB19(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->c);
	return 8;
}

// rr d
int CPU::op_CB1A(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->d);
	return 8;
}

// rrc e
int CPU::op_CB1B(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->e);
	return 8;
}

// rr h
int CPU::op_CB1C(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->h);
	return 8;
}

// rr l
int CPU::op_CB1D(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->l);
	return 8;
}

// rr (hl)
int CPU::op_CB1E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RR(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// rr a
int CPU::op_CB1F(CPURegisters * registers)
{
	op_CB_RR(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 20 through CB 27.
// Performs SLA on the given value.
void CPU::op_CB_SLA(CPURegisters *registers, int *value)
{
	if ((*value >> 7) == 1)
	{
		registers->f = 0b00010000;
	}
	else
	{
		registers->f = 0b00000000;
	}

	*value = ((*value << 1) & 0xff);
	if (*value == 0)
	{
		registers->f |= 0b10000000;
	}
}

// sla b
int CPU::op_CB20(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->b);
	return 8;
}

// sla c
int CPU::op_CB21(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->c);
	return 8;
}

// sla d
int CPU::op_CB22(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->d);
	return 8;
}

// sla e
int CPU::op_CB23(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->e);
	return 8;
}

// sla h
int CPU::op_CB24(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->h);
	return 8;
}

// sla l
int CPU::op_CB25(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->l);
	return 8;
}

// sla (hl)
int CPU::op_CB26(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SLA(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// sla a
int CPU::op_CB27(CPURegisters * registers)
{
	op_CB_SLA(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 28 through CB 2F.
// Performs SRA on the given value.
void CPU::op_CB_SRA(CPURegisters *registers, int *value)
{
	int msb = (*value & 0b10000000);
	if ((*value & 1) == 1)
	{
		registers->f = 0b00010000;
	}
	else
	{
		registers->f = 0b00000000;
	}

	*value = (((*value >> 1) & 0xff) | msb);
	if (*value == 0)
	{
		registers->f |= 0b10000000;
	}
}

// sra b
int CPU::op_CB28(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->b);
	return 8;
}

// sra c
int CPU::op_CB29(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->c);
	return 8;
}

// sra d
int CPU::op_CB2A(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->d);
	return 8;
}

// sra e
int CPU::op_CB2B(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->e);
	return 8;
}

// sra h
int CPU::op_CB2C(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->h);
	return 8;
}

// sra l
int CPU::op_CB2D(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->l);
	return 8;
}

// sra (hl)
int CPU::op_CB2E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SRA(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// sra a
int CPU::op_CB2F(CPURegisters * registers)
{
	op_CB_SRA(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 30 through CB 37.
// Performs SWAP on the given value.
void CPU::op_CB_SWAP(CPURegisters *registers, int *value)
{
	int lowerNibble = (*value & 0xf);
	*value = ((*value >> 4) | (lowerNibble << 4));
	if (*value == 0)
	{
		registers->f = 0b10000000;
	}
	else
	{
		registers->f = 0b00000000;
	}
}

// swap b
int CPU::op_CB30(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->b);
	return 8;
}

// swap c
int CPU::op_CB31(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->c);
	return 8;
}

// swap d
int CPU::op_CB32(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->d);
	return 8;
}

// swap e
int CPU::op_CB33(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->e);
	return 8;
}

// swap h
int CPU::op_CB34(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->h);
	return 8;
}

// swap l
int CPU::op_CB35(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->l);
	return 8;
}

// swap (hl)
int CPU::op_CB36(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SWAP(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// swap a
int CPU::op_CB37(CPURegisters * registers)
{
	op_CB_SWAP(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 38 through CB 3F.
// Performs SRL on the given value.
void CPU::op_CB_SRL(CPURegisters *registers, int *value)
{
	int newCarry = (*value & 1);
	*value = (*value >> 1);
	if (*value == 0)
	{
		registers->f = 0b10000000;
	}
	else
	{
		registers->f = 0b00000000;
	}

	registers->f |= (newCarry << 4);
}

// srl b
int CPU::op_CB38(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->b);
	return 8;
}

// srl c
int CPU::op_CB39(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->c);
	return 8;
}

// srl d
int CPU::op_CB3A(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->d);
	return 8;
}

// srl e
int CPU::op_CB3B(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->e);
	return 8;
}

// srl h
int CPU::op_CB3C(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->h);
	return 8;
}

// srl l
int CPU::op_CB3D(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->l);
	return 8;
}

// srl (hl)
int CPU::op_CB3E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SRL(registers, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// srl a
int CPU::op_CB3F(CPURegisters * registers)
{
	op_CB_SRL(registers, &registers->a);
	return 8;
}

// Helper for opcodes CB 40 through CB 7F.
// Performs BIT on the given value.
void CPU::op_CB_BIT(CPURegisters *registers, int bitIndex, int *value)
{
	if ((*value & (1 << bitIndex)) == 0)
	{
		registers->f |= 0b10000000;
	}
	else
	{
		registers->f &= 0b01111111;
	}

	registers->f |= 0b00100000;
	registers->f &= 0b10111111;
}

// bit 0, b
int CPU::op_CB40(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->b);
	return 8;
}

// bit 0, c
int CPU::op_CB41(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->c);
	return 8;
}

// bit 0, d
int CPU::op_CB42(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->d);
	return 8;
}

// bit 0, e
int CPU::op_CB43(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->e);
	return 8;
}

// bit 0, h
int CPU::op_CB44(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->h);
	return 8;
}

// bit 0, l
int CPU::op_CB45(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->l);
	return 8;
}

// bit 0, (hl)
int CPU::op_CB46(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 0, &value);
	return 8;
}

// bit 0, a
int CPU::op_CB47(CPURegisters * registers)
{
	op_CB_BIT(registers, 0, &registers->a);
	return 8;
}

// bit 1, b
int CPU::op_CB48(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->b);
	return 8;
}

// bit 1, c
int CPU::op_CB49(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->c);
	return 8;
}

// bit 1, d
int CPU::op_CB4A(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->d);
	return 8;
}

// bit 1, e
int CPU::op_CB4B(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->e);
	return 8;
}

// bit 1, h
int CPU::op_CB4C(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->h);
	return 8;
}

// bit 1, l
int CPU::op_CB4D(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->l);
	return 8;
}

// bit 1, (hl)
int CPU::op_CB4E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 1, &value);
	return 8;
}

// bit 1, a
int CPU::op_CB4F(CPURegisters * registers)
{
	op_CB_BIT(registers, 1, &registers->a);
	return 8;
}

// bit 2, b
int CPU::op_CB50(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->b);
	return 8;
}

// bit 2, c
int CPU::op_CB51(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->c);
	return 8;
}

// bit 2, d
int CPU::op_CB52(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->d);
	return 8;
}

// bit 2, e
int CPU::op_CB53(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->e);
	return 8;
}

// bit 2, h
int CPU::op_CB54(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->h);
	return 8;
}

// bit 2, l
int CPU::op_CB55(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->l);
	return 8;
}

// bit 2, (hl)
int CPU::op_CB56(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 2, &value);
	return 8;
}

// bit 2, a
int CPU::op_CB57(CPURegisters * registers)
{
	op_CB_BIT(registers, 2, &registers->a);
	return 8;
}

// bit 3, b
int CPU::op_CB58(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->b);
	return 8;
}

// bit 3, c
int CPU::op_CB59(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->c);
	return 8;
}

// bit 3, d
int CPU::op_CB5A(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->d);
	return 8;
}

// bit 3, e
int CPU::op_CB5B(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->e);
	return 8;
}

// bit 3, h
int CPU::op_CB5C(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->h);
	return 8;
}

// bit 3, l
int CPU::op_CB5D(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->l);
	return 8;
}

// bit 3, (hl)
int CPU::op_CB5E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 3, &value);
	return 8;
}

// bit 3, a
int CPU::op_CB5F(CPURegisters * registers)
{
	op_CB_BIT(registers, 3, &registers->a);
	return 8;
}

// bit 4, b
int CPU::op_CB60(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->b);
	return 8;
}

// bit 4, c
int CPU::op_CB61(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->c);
	return 8;
}

// bit 4, d
int CPU::op_CB62(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->d);
	return 8;
}

// bit 4, e
int CPU::op_CB63(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->e);
	return 8;
}

// bit 4, h
int CPU::op_CB64(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->h);
	return 8;
}

// bit 4, l
int CPU::op_CB65(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->l);
	return 8;
}

// bit 4, (hl)
int CPU::op_CB66(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 4, &value);
	return 8;
}

// bit 4, a
int CPU::op_CB67(CPURegisters * registers)
{
	op_CB_BIT(registers, 4, &registers->a);
	return 8;
}

// bit 5, b
int CPU::op_CB68(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->b);
	return 8;
}

// bit 5, c
int CPU::op_CB69(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->c);
	return 8;
}

// bit 5, d
int CPU::op_CB6A(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->d);
	return 8;
}

// bit 5, e
int CPU::op_CB6B(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->e);
	return 8;
}

// bit 5, h
int CPU::op_CB6C(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->h);
	return 8;
}

// bit 5, l
int CPU::op_CB6D(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->l);
	return 8;
}

// bit 5, (hl)
int CPU::op_CB6E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 5, &value);
	return 8;
}

// bit 5, a
int CPU::op_CB6F(CPURegisters * registers)
{
	op_CB_BIT(registers, 5, &registers->a);
	return 8;
}

// bit 6, b
int CPU::op_CB70(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->b);
	return 8;
}

// bit 6, c
int CPU::op_CB71(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->c);
	return 8;
}

// bit 6, d
int CPU::op_CB72(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->d);
	return 8;
}

// bit 6, e
int CPU::op_CB73(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->e);
	return 8;
}

// bit 6, h
int CPU::op_CB74(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->h);
	return 8;
}

// bit 6, l
int CPU::op_CB75(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->l);
	return 8;
}

// bit 6, (hl)
int CPU::op_CB76(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 6, &value);
	return 8;
}

// bit 6, a
int CPU::op_CB77(CPURegisters * registers)
{
	op_CB_BIT(registers, 6, &registers->a);
	return 8;
}

// bit 7, b
int CPU::op_CB78(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->b);
	return 8;
}

// bit 7, c
int CPU::op_CB79(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->c);
	return 8;
}

// bit 7, d
int CPU::op_CB7A(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->d);
	return 8;
}

// bit 7, e
int CPU::op_CB7B(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->e);
	return 8;
}

// bit 7, h
int CPU::op_CB7C(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->h);
	return 8;
}

// bit 7, l
int CPU::op_CB7D(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->l);
	return 8;
}

// bit 7, (hl)
int CPU::op_CB7E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	op_CB_BIT(registers, 7, &value);
	return 8;
}

// bit 7, a
int CPU::op_CB7F(CPURegisters * registers)
{
	op_CB_BIT(registers, 7, &registers->a);
	return 8;
}

// Helper for opcodes CB 80 through CB BF.
// Performs RES on the given value.
void CPU::op_CB_RES(CPURegisters *registers, int bitIndex, int *value)
{
	int mask = (~(1 << bitIndex) & 0xff);
	*value &= mask;
}

// res 0, b
int CPU::op_CB80(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->b);
	return 8;
}

// res 0, c
int CPU::op_CB81(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->c);
	return 8;
}

// res 0, d
int CPU::op_CB82(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->d);
	return 8;
}

// res 0, e
int CPU::op_CB83(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->e);
	return 8;
}

// res 0, h
int CPU::op_CB84(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->h);
	return 8;
}

// res 0, l
int CPU::op_CB85(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->l);
	return 8;
}

// res 0, (hl)
int CPU::op_CB86(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 0, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 0, a
int CPU::op_CB87(CPURegisters * registers)
{
	op_CB_RES(registers, 0, &registers->a);
	return 8;
}

// res 1, b
int CPU::op_CB88(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->b);
	return 8;
}

// res 1, c
int CPU::op_CB89(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->c);
	return 8;
}

// res 1, d
int CPU::op_CB8A(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->d);
	return 8;
}

// res 1, e
int CPU::op_CB8B(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->e);
	return 8;
}

// res 1, h
int CPU::op_CB8C(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->h);
	return 8;
}

// res 1, l
int CPU::op_CB8D(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->l);
	return 8;
}

// res 1, (hl)
int CPU::op_CB8E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 1, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 1, a
int CPU::op_CB8F(CPURegisters * registers)
{
	op_CB_RES(registers, 1, &registers->a);
	return 8;
}

// res 2, b
int CPU::op_CB90(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->b);
	return 8;
}

// res 2, c
int CPU::op_CB91(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->c);
	return 8;
}

// res 2, d
int CPU::op_CB92(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->d);
	return 8;
}

// res 2, e
int CPU::op_CB93(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->e);
	return 8;
}

// res 2, h
int CPU::op_CB94(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->h);
	return 8;
}

// res 2, l
int CPU::op_CB95(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->l);
	return 8;
}

// res 2, (hl)
int CPU::op_CB96(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 2, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 2, a
int CPU::op_CB97(CPURegisters * registers)
{
	op_CB_RES(registers, 2, &registers->a);
	return 8;
}

// res 3, b
int CPU::op_CB98(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->b);
	return 8;
}

// res 3, c
int CPU::op_CB99(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->c);
	return 8;
}

// res 3, d
int CPU::op_CB9A(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->d);
	return 8;
}

// res 3, e
int CPU::op_CB9B(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->e);
	return 8;
}

// res 3, h
int CPU::op_CB9C(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->h);
	return 8;
}

// res 3, l
int CPU::op_CB9D(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->l);
	return 8;
}

// res 3, (hl)
int CPU::op_CB9E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 3, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 3, a
int CPU::op_CB9F(CPURegisters * registers)
{
	op_CB_RES(registers, 3, &registers->a);
	return 8;
}

// res 4, b
int CPU::op_CBA0(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->b);
	return 8;
}

// res 4, c
int CPU::op_CBA1(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->c);
	return 8;
}

// res 4, d
int CPU::op_CBA2(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->d);
	return 8;
}

// res 4, e
int CPU::op_CBA3(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->e);
	return 8;
}

// res 4, h
int CPU::op_CBA4(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->h);
	return 8;
}

// res 4, l
int CPU::op_CBA5(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->l);
	return 8;
}

// res 4, (hl)
int CPU::op_CBA6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 4, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 4, a
int CPU::op_CBA7(CPURegisters * registers)
{
	op_CB_RES(registers, 4, &registers->a);
	return 8;
}

// res 5, b
int CPU::op_CBA8(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->b);
	return 8;
}

// res 5, c
int CPU::op_CBA9(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->c);
	return 8;
}

// res 5, d
int CPU::op_CBAA(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->d);
	return 8;
}

// res 5, e
int CPU::op_CBAB(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->e);
	return 8;
}

// res 5, h
int CPU::op_CBAC(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->h);
	return 8;
}

// res 5, l
int CPU::op_CBAD(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->l);
	return 8;
}

// res 5, (hl)
int CPU::op_CBAE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 5, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 5, a
int CPU::op_CBAF(CPURegisters * registers)
{
	op_CB_RES(registers, 5, &registers->a);
	return 8;
}

// res 6, b
int CPU::op_CBB0(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->b);
	return 8;
}

// res 6, c
int CPU::op_CBB1(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->c);
	return 8;
}

// res 6, d
int CPU::op_CBB2(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->d);
	return 8;
}

// res 6, e
int CPU::op_CBB3(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->e);
	return 8;
}

// res 6, h
int CPU::op_CBB4(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->h);
	return 8;
}

// res 6, l
int CPU::op_CBB5(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->l);
	return 8;
}

// res 6, (hl)
int CPU::op_CBB6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 6, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 6, a
int CPU::op_CBB7(CPURegisters * registers)
{
	op_CB_RES(registers, 6, &registers->a);
	return 8;
}

// res 7, b
int CPU::op_CBB8(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->b);
	return 8;
}

// res 7, c
int CPU::op_CBB9(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->c);
	return 8;
}

// res 7, d
int CPU::op_CBBA(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->d);
	return 8;
}

// res 7, e
int CPU::op_CBBB(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->e);
	return 8;
}

// res 7, h
int CPU::op_CBBC(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->h);
	return 8;
}

// res 7, l
int CPU::op_CBBD(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->l);
	return 8;
}

// res 7, (hl)
int CPU::op_CBBE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_RES(registers, 7, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// res 7, a
int CPU::op_CBBF(CPURegisters * registers)
{
	op_CB_RES(registers, 7, &registers->a);
	return 8;
}

// Helper for opcodes CB C0 through CB FF.
// Performs SET on the given value.
void CPU::op_CB_SET(CPURegisters *registers, int bitIndex, int *value)
{
	*value |= (1 << bitIndex);
}

// set 0, b
int CPU::op_CBC0(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->b);
	return 8;
}

// set 0, c
int CPU::op_CBC1(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->c);
	return 8;
}

// set 0, d
int CPU::op_CBC2(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->d);
	return 8;
}

// set 0, e
int CPU::op_CBC3(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->e);
	return 8;
}

// set 0, h
int CPU::op_CBC4(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->h);
	return 8;
}

// set 0, l
int CPU::op_CBC5(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->l);
	return 8;
}

// set 0, (hl)
int CPU::op_CBC6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 0, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 0, a
int CPU::op_CBC7(CPURegisters * registers)
{
	op_CB_SET(registers, 0, &registers->a);
	return 8;
}

// set 1, b
int CPU::op_CBC8(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->b);
	return 8;
}

// set 1, c
int CPU::op_CBC9(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->c);
	return 8;
}

// set 1, d
int CPU::op_CBCA(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->d);
	return 8;
}

// set 1, e
int CPU::op_CBCB(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->e);
	return 8;
}

// set 1, h
int CPU::op_CBCC(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->h);
	return 8;
}

// set 1, l
int CPU::op_CBCD(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->l);
	return 8;
}

// set 1, (hl)
int CPU::op_CBCE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 1, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 1, a
int CPU::op_CBCF(CPURegisters * registers)
{
	op_CB_SET(registers, 1, &registers->a);
	return 8;
}

// set 2, b
int CPU::op_CBD0(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->b);
	return 8;
}

// set 2, c
int CPU::op_CBD1(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->c);
	return 8;
}

// set 2, d
int CPU::op_CBD2(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->d);
	return 8;
}

// set 2, e
int CPU::op_CBD3(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->e);
	return 8;
}

// set 2, h
int CPU::op_CBD4(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->h);
	return 8;
}

// set 2, l
int CPU::op_CBD5(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->l);
	return 8;
}

// set 2, (hl)
int CPU::op_CBD6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 2, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 2, a
int CPU::op_CBD7(CPURegisters * registers)
{
	op_CB_SET(registers, 2, &registers->a);
	return 8;
}

// set 3, b
int CPU::op_CBD8(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->b);
	return 8;
}

// set 3, c
int CPU::op_CBD9(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->c);
	return 8;
}

// set 3, d
int CPU::op_CBDA(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->d);
	return 8;
}

// set 3, e
int CPU::op_CBDB(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->e);
	return 8;
}

// set 3, h
int CPU::op_CBDC(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->h);
	return 8;
}

// set 3, l
int CPU::op_CBDD(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->l);
	return 8;
}

// set 3, (hl)
int CPU::op_CBDE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 3, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 3, a
int CPU::op_CBDF(CPURegisters * registers)
{
	op_CB_SET(registers, 3, &registers->a);
	return 8;
}

// set 4, b
int CPU::op_CBE0(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->b);
	return 8;
}

// set 4, c
int CPU::op_CBE1(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->c);
	return 8;
}

// set 4, d
int CPU::op_CBE2(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->d);
	return 8;
}

// set 4, e
int CPU::op_CBE3(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->e);
	return 8;
}

// set 4, h
int CPU::op_CBE4(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->h);
	return 8;
}

// set 4, l
int CPU::op_CBE5(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->l);
	return 8;
}

// set 4, (hl)
int CPU::op_CBE6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 4, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 4, a
int CPU::op_CBE7(CPURegisters * registers)
{
	op_CB_SET(registers, 4, &registers->a);
	return 8;
}

// set 5, b
int CPU::op_CBE8(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->b);
	return 8;
}

// set 5, c
int CPU::op_CBE9(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->c);
	return 8;
}

// set 5, d
int CPU::op_CBEA(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->d);
	return 8;
}

// set 5, e
int CPU::op_CBEB(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->e);
	return 8;
}

// set 5, h
int CPU::op_CBEC(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->h);
	return 8;
}

// set 5, l
int CPU::op_CBED(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->l);
	return 8;
}

// set 5, (hl)
int CPU::op_CBEE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 5, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 5, a
int CPU::op_CBEF(CPURegisters * registers)
{
	op_CB_SET(registers, 5, &registers->a);
	return 8;
}

// set 6, b
int CPU::op_CBF0(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->b);
	return 8;
}

// set 6, c
int CPU::op_CBF1(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->c);
	return 8;
}

// set 6, d
int CPU::op_CBF2(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->d);
	return 8;
}

// set 6, e
int CPU::op_CBF3(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->e);
	return 8;
}

// set 6, h
int CPU::op_CBF4(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->h);
	return 8;
}

// set 6, l
int CPU::op_CBF5(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->l);
	return 8;
}

// set 6, (hl)
int CPU::op_CBF6(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 6, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 6, a
int CPU::op_CBF7(CPURegisters * registers)
{
	op_CB_SET(registers, 6, &registers->a);
	return 8;
}

// set 7, b
int CPU::op_CBF8(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->b);
	return 8;
}

// set 7, c
int CPU::op_CBF9(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->c);
	return 8;
}

// set 7, d
int CPU::op_CBFA(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->d);
	return 8;
}

// set 7, e
int CPU::op_CBFB(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->e);
	return 8;
}

// set 7, h
int CPU::op_CBFC(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->h);
	return 8;
}

// set 7, l
int CPU::op_CBFD(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->l);
	return 8;
}

// set 7, (hl)
int CPU::op_CBFE(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	TickTimer(4);
	int value = mmu->ReadByte(hl);
	TickTimer(4);
	op_CB_SET(registers, 7, &value);
	mmu->WriteByte(value, hl);
	return 8;
}

// set 7, a
int CPU::op_CBFF(CPURegisters * registers)
{
	op_CB_SET(registers, 7, &registers->a);
	return 8;
}