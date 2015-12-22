#include "stdafx.h"
#include "CPUOpcodes.h"

CPUOpcodes::CPUOpcodes(IMMU *mmu)
{
	CPUOpcodes::mmu = mmu;
}


CPUOpcodes::~CPUOpcodes()
{
}

// NOP
int CPUOpcodes::op_00(CPURegisters *registers)
{
	return 4;
}

// ld bc, d16
int CPUOpcodes::op_01(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->b = (word >> 8) & 0xff;
	registers->c = word & 0xff;
	return 12;
}

// ld (bc), a
int CPUOpcodes::op_02(CPURegisters * registers)
{
	int address = (registers->b << 8) | registers->c;
	mmu->WriteByte(registers->a, address);
	return 8;
}

// inc bc
int CPUOpcodes::op_03(CPURegisters * registers)
{
	int word = (registers->b << 8) | registers->c;
	word++;
	registers->b = (word >> 8) & 0xff;
	registers->c = word & 0xff;
	return 8;
}

// inc b
int CPUOpcodes::op_04(CPURegisters * registers)
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
int CPUOpcodes::op_05(CPURegisters * registers)
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

	if (registers->b & 0x0f == 0x0f)
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
int CPUOpcodes::op_06(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->b = byte;
	return 8;
}

// rlca
int CPUOpcodes::op_07(CPURegisters * registers)
{
	int carry = (registers->a >> 7);
	registers->a = ((registers->a << 1) & 0xff) | carry;
	registers->f = carry << 4;
	return 4;
}

// ld (a16), sp
int CPUOpcodes::op_08(CPURegisters * registers)
{
	int address = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	mmu->WriteWord(registers->sp, address);
	return 20;
}

// add hl, bc
int CPUOpcodes::op_09(CPURegisters * registers)
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
int CPUOpcodes::op_0A(CPURegisters * registers)
{
	int address = (registers->b << 8) | registers->c;
	registers->a = mmu->ReadByte(address);
	return 8;
}

// dec bc
int CPUOpcodes::op_0B(CPURegisters * registers)
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
int CPUOpcodes::op_0C(CPURegisters * registers)
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
int CPUOpcodes::op_0D(CPURegisters * registers)
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

	if (registers->c & 0x0f == 0x0f)
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
int CPUOpcodes::op_0E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->c = byte;
	return 8;
}

// rrca
int CPUOpcodes::op_0F(CPURegisters * registers)
{
	int carry = (registers->a & 1);
	registers->a = (registers->a >> 1) | (carry << 7);
	registers->f = carry << 4;
	return 4;
}

// stop
int CPUOpcodes::op_10(CPURegisters * registers)
{
	// TODO: implement halt cpu
	return 0;
}

// ld de, d16
int CPUOpcodes::op_11(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->d = (word >> 8) & 0xff;
	registers->e = word & 0xff;
	return 12;
}

// ld (de), a
int CPUOpcodes::op_12(CPURegisters * registers)
{
	int address = (registers->d << 8) | registers->e;
	mmu->WriteByte(registers->a, address);
	return 8;
}

// inc de
int CPUOpcodes::op_13(CPURegisters * registers)
{
	int de = (registers->d << 8) | registers->e;
	de++;
	registers->d = (de >> 8) & 0xff;
	registers->e = de & 0xff;
	return 8;
}

// inc d
int CPUOpcodes::op_14(CPURegisters * registers)
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
int CPUOpcodes::op_15(CPURegisters * registers)
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

	if (registers->d & 0x0f == 0x0f)
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
int CPUOpcodes::op_16(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->d = byte;
	return 8;
}

// rla
int CPUOpcodes::op_17(CPURegisters * registers)
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
int CPUOpcodes::op_18(CPURegisters * registers)
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
int CPUOpcodes::op_19(CPURegisters * registers)
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
int CPUOpcodes::op_1A(CPURegisters * registers)
{
	int address = (registers->d << 8) | registers->e;
	registers->a = mmu->ReadByte(address);
	return 8;
}

// dec de
int CPUOpcodes::op_1B(CPURegisters * registers)
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
int CPUOpcodes::op_1C(CPURegisters * registers)
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
int CPUOpcodes::op_1D(CPURegisters * registers)
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

	if (registers->e & 0x0f == 0x0f)
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
int CPUOpcodes::op_1E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->e = byte;
	return 8;
}

// rra
int CPUOpcodes::op_1F(CPURegisters * registers)
{
	int carry = (registers->f >> 4) & 1;
	int newCarry = (registers->a & 1);
	registers->a = (registers->a >> 1) | (carry << 7);
	registers->f = (newCarry << 4);
	return 4;
}
