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

// jr nz, r8
int CPUOpcodes::op_20(CPURegisters * registers)
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
int CPUOpcodes::op_21(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->h = (word >> 8) & 0xff;
	registers->l = word & 0xff;
	return 12;
}

// ld (hl+), a
int CPUOpcodes::op_22(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->a, hl);
	hl++;
	registers->h = (hl >> 8) & 0xff;
	registers->l = (hl & 0xff);
	return 8;
}

// inc hl
int CPUOpcodes::op_23(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	hl++;
	registers->h = (hl >> 8) & 0xff;
	registers->l = hl & 0xff;
	return 8;
}

// inc h
int CPUOpcodes::op_24(CPURegisters * registers)
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
int CPUOpcodes::op_25(CPURegisters * registers)
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
int CPUOpcodes::op_26(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->h = byte;
	return 8;
}

// daa
// See table here: http://www.z80.info/z80syntx.htm#DAA
int CPUOpcodes::op_27(CPURegisters * registers)
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
int CPUOpcodes::op_28(CPURegisters * registers)
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
int CPUOpcodes::op_29(CPURegisters * registers)
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
int CPUOpcodes::op_2A(CPURegisters * registers)
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
int CPUOpcodes::op_2B(CPURegisters * registers)
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
int CPUOpcodes::op_2C(CPURegisters * registers)
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
int CPUOpcodes::op_2D(CPURegisters * registers)
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

	if (registers->l & 0x0f == 0x0f)
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
int CPUOpcodes::op_2E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->l = byte;
	return 8;
}

// cpl
int CPUOpcodes::op_2F(CPURegisters * registers)
{
	registers->a = (~registers->a & 0xff);
	registers->f |= 0b01100000;
	return 4;
}

// jr nc, r8
int CPUOpcodes::op_30(CPURegisters * registers)
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
int CPUOpcodes::op_31(CPURegisters * registers)
{
	int word = mmu->ReadWord(registers->pc);
	registers->pc += 2;
	registers->sp = word;
	return 12;
}

// ld (hl-), a
int CPUOpcodes::op_32(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->a, hl);
	hl--;
	registers->h = (hl >> 8) & 0xff;
	registers->l = (hl & 0xff);
	return 8;
}

// inc sp
int CPUOpcodes::op_33(CPURegisters * registers)
{
	registers->sp = (registers->sp + 1) & 0xffff;
	return 8;
}

// inc (hl)
int CPUOpcodes::op_34(CPURegisters * registers)
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

	mmu->WriteByte(value, hl);
	return 12;
}

// dec (hl)
int CPUOpcodes::op_35(CPURegisters * registers)
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
	
	mmu->WriteByte(value, hl);
	return 12;
}

// ld (hl), d8
int CPUOpcodes::op_36(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(byte, hl);
	return 12;
}

// scf
int CPUOpcodes::op_37(CPURegisters * registers)
{
	registers->f &= 0b10011111;
	registers->f |= 0b00010000;
	return 4;
}

// jr c, r8
int CPUOpcodes::op_38(CPURegisters * registers)
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
int CPUOpcodes::op_39(CPURegisters * registers)
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
int CPUOpcodes::op_3A(CPURegisters * registers)
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
int CPUOpcodes::op_3B(CPURegisters * registers)
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
int CPUOpcodes::op_3C(CPURegisters * registers)
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
int CPUOpcodes::op_3D(CPURegisters * registers)
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

	if (registers->a & 0x0f == 0x0f)
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
int CPUOpcodes::op_3E(CPURegisters * registers)
{
	int byte = mmu->ReadByte(registers->pc);
	registers->pc++;
	registers->a = byte;
	return 8;
}

// ccf
int CPUOpcodes::op_3F(CPURegisters * registers)
{
	registers->f ^= 0b00010000;
	registers->f = (registers->f & 0b10011111) & 0xff;
	return 4;
}

// ld b, b
int CPUOpcodes::op_40(CPURegisters * registers)
{
	return 4;
}

// ld b, c
int CPUOpcodes::op_41(CPURegisters * registers)
{
	registers->b = registers->c;
	return 4;
}

// ld b, d
int CPUOpcodes::op_42(CPURegisters * registers)
{
	registers->b = registers->d;
	return 4;
}

// ld b, e
int CPUOpcodes::op_43(CPURegisters * registers)
{
	registers->b = registers->e;
	return 4;
}

// ld b, h
int CPUOpcodes::op_44(CPURegisters * registers)
{
	registers->b = registers->h;
	return 4;
}

// ld b, l
int CPUOpcodes::op_45(CPURegisters * registers)
{
	registers->b = registers->l;
	return 4;
}

// ld b, (hl)
int CPUOpcodes::op_46(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->b = mmu->ReadByte(hl);
	return 8;
}

// ld b, a
int CPUOpcodes::op_47(CPURegisters * registers)
{
	registers->b = registers->a;
	return 4;
}

// ld c, b
int CPUOpcodes::op_48(CPURegisters * registers)
{
	registers->c = registers->b;
	return 4;
}

// ld c, c
int CPUOpcodes::op_49(CPURegisters * registers)
{
	return 4;
}

// ld c, d
int CPUOpcodes::op_4A(CPURegisters * registers)
{
	registers->c = registers->d;
	return 4;
}

// ld c, e
int CPUOpcodes::op_4B(CPURegisters * registers)
{
	registers->c = registers->e;
	return 4;
}

// ld c, h
int CPUOpcodes::op_4C(CPURegisters * registers)
{
	registers->c = registers->h;
	return 4;
}

// ld c, l
int CPUOpcodes::op_4D(CPURegisters * registers)
{
	registers->c = registers->l;
	return 4;
}

// ld c, (hl)
int CPUOpcodes::op_4E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->c = mmu->ReadByte(hl);
	return 8;
}

// ld c, a
int CPUOpcodes::op_4F(CPURegisters * registers)
{
	registers->c = registers->a;
	return 4;
}

// ld d, b
int CPUOpcodes::op_50(CPURegisters * registers)
{
	registers->d = registers->b;
	return 4;
}

// ld d, c
int CPUOpcodes::op_51(CPURegisters * registers)
{
	registers->d = registers->c;
	return 4;
}

// ld d, d
int CPUOpcodes::op_52(CPURegisters * registers)
{
	return 4;
}

// ld d, e
int CPUOpcodes::op_53(CPURegisters * registers)
{
	registers->d = registers->e;
	return 4;
}

// ld d, h
int CPUOpcodes::op_54(CPURegisters * registers)
{
	registers->d = registers->h;
	return 4;
}

// ld d, l
int CPUOpcodes::op_55(CPURegisters * registers)
{
	registers->d = registers->l;
	return 4;
}

// ld d, (hl)
int CPUOpcodes::op_56(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->d = mmu->ReadByte(hl);
	return 8;
}

// ld d, a
int CPUOpcodes::op_57(CPURegisters * registers)
{
	registers->d = registers->a;
	return 4;
}

// ld e, b
int CPUOpcodes::op_58(CPURegisters * registers)
{
	registers->e = registers->b;
	return 4;
}

// ld e, c
int CPUOpcodes::op_59(CPURegisters * registers)
{
	registers->e = registers->c;
	return 4;
}

// ld e, d
int CPUOpcodes::op_5A(CPURegisters * registers)
{
	registers->e = registers->d;
	return 4;
}

// ld e, e
int CPUOpcodes::op_5B(CPURegisters * registers)
{
	return 4;
}

// ld e, h
int CPUOpcodes::op_5C(CPURegisters * registers)
{
	registers->e = registers->h;
	return 4;
}

// ld e, l
int CPUOpcodes::op_5D(CPURegisters * registers)
{
	registers->e = registers->l;
	return 4;
}

// ld e, (hl)
int CPUOpcodes::op_5E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->e = mmu->ReadByte(hl);
	return 8;
}

// ld e, a
int CPUOpcodes::op_5F(CPURegisters * registers)
{
	registers->e = registers->a;
	return 4;
}

// ld h, b
int CPUOpcodes::op_60(CPURegisters * registers)
{
	registers->h = registers->b;
	return 4;
}

// ld h, c
int CPUOpcodes::op_61(CPURegisters * registers)
{
	registers->h = registers->c;
	return 4;
}

// ld h, d
int CPUOpcodes::op_62(CPURegisters * registers)
{
	registers->h = registers->d;
	return 4;
}

// ld h, e
int CPUOpcodes::op_63(CPURegisters * registers)
{
	registers->h = registers->e;
	return 4;
}

// ld h, h
int CPUOpcodes::op_64(CPURegisters * registers)
{
	return 4;
}

// ld h, l
int CPUOpcodes::op_65(CPURegisters * registers)
{
	registers->h = registers->l;
	return 4;
}

// ld h, (hl)
int CPUOpcodes::op_66(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->h = mmu->ReadByte(hl);
	return 8;
}

// ld h, a
int CPUOpcodes::op_67(CPURegisters * registers)
{
	registers->h = registers->a;
	return 4;
}

// ld l, b
int CPUOpcodes::op_68(CPURegisters * registers)
{
	registers->l = registers->b;
	return 4;
}

// ld l, c
int CPUOpcodes::op_69(CPURegisters * registers)
{
	registers->l = registers->c;
	return 4;
}

// ld l, d
int CPUOpcodes::op_6A(CPURegisters * registers)
{
	registers->l = registers->d;
	return 4;
}

// ld l, e
int CPUOpcodes::op_6B(CPURegisters * registers)
{
	registers->l = registers->e;
	return 4;
}

// ld l, h
int CPUOpcodes::op_6C(CPURegisters * registers)
{
	registers->l = registers->h;
	return 4;
}

// ld l, l
int CPUOpcodes::op_6D(CPURegisters * registers)
{
	return 4;
}

// ld l, (hl)
int CPUOpcodes::op_6E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->l = mmu->ReadByte(hl);
	return 8;
}

// ld l, a
int CPUOpcodes::op_6F(CPURegisters * registers)
{
	registers->l = registers->a;
	return 4;
}

// ld (hl), b
int CPUOpcodes::op_70(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->b, hl);
	return 8;
}

// ld (hl), c
int CPUOpcodes::op_71(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->c, hl);
	return 8;
}

// ld (hl), d
int CPUOpcodes::op_72(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->d, hl);
	return 8;
}

// ld (hl), e
int CPUOpcodes::op_73(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->e, hl);
	return 8;
}

// ld (hl), h
int CPUOpcodes::op_74(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->h, hl);
	return 8;
}

// ld (hl), l
int CPUOpcodes::op_75(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->l, hl);
	return 8;
}

// halt
int CPUOpcodes::op_76(CPURegisters * registers)
{
	// TODO: implement halt
	return 4;
}

// ld (hl), a
int CPUOpcodes::op_77(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	mmu->WriteByte(registers->a, hl);
	return 8;
}

// ld a, b
int CPUOpcodes::op_78(CPURegisters * registers)
{
	registers->a = registers->b;
	return 4;
}

// ld a, c
int CPUOpcodes::op_79(CPURegisters * registers)
{
	registers->a = registers->c;
	return 4;
}

// ld a, d
int CPUOpcodes::op_7A(CPURegisters * registers)
{
	registers->a = registers->d;
	return 4;
}

// ld a, e
int CPUOpcodes::op_7B(CPURegisters * registers)
{
	registers->a = registers->e;
	return 4;
}

// ld a, h
int CPUOpcodes::op_7C(CPURegisters * registers)
{
	registers->a = registers->h;
	return 4;
}

// ld a, l
int CPUOpcodes::op_7D(CPURegisters * registers)
{
	registers->a = registers->l;
	return 4;
}

// ld a, (hl)
int CPUOpcodes::op_7E(CPURegisters * registers)
{
	int hl = (registers->h << 8) | registers->l;
	registers->a = mmu->ReadByte(hl);
	return 8;
}

// ld a, a
int CPUOpcodes::op_7F(CPURegisters * registers)
{
	return 4;
}

// add a, b	
int CPUOpcodes::op_80(CPURegisters * registers)
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
int CPUOpcodes::op_81(CPURegisters * registers)
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
int CPUOpcodes::op_82(CPURegisters * registers)
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
int CPUOpcodes::op_83(CPURegisters * registers)
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
int CPUOpcodes::op_84(CPURegisters * registers)
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
int CPUOpcodes::op_85(CPURegisters * registers)
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
int CPUOpcodes::op_86(CPURegisters * registers)
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
int CPUOpcodes::op_87(CPURegisters * registers)
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
int CPUOpcodes::op_88(CPURegisters * registers)
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
int CPUOpcodes::op_89(CPURegisters * registers)
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
int CPUOpcodes::op_8A(CPURegisters * registers)
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
int CPUOpcodes::op_8B(CPURegisters * registers)
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
int CPUOpcodes::op_8C(CPURegisters * registers)
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
int CPUOpcodes::op_8D(CPURegisters * registers)
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
int CPUOpcodes::op_8E(CPURegisters * registers)
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
int CPUOpcodes::op_8F(CPURegisters * registers)
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
int CPUOpcodes::op_90(CPURegisters * registers)
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
int CPUOpcodes::op_91(CPURegisters * registers)
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
int CPUOpcodes::op_92(CPURegisters * registers)
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
int CPUOpcodes::op_93(CPURegisters * registers)
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
int CPUOpcodes::op_94(CPURegisters * registers)
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
int CPUOpcodes::op_95(CPURegisters * registers)
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
int CPUOpcodes::op_96(CPURegisters * registers)
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
int CPUOpcodes::op_97(CPURegisters * registers)
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
int CPUOpcodes::op_98(CPURegisters * registers)
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
int CPUOpcodes::op_99(CPURegisters * registers)
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
int CPUOpcodes::op_9A(CPURegisters * registers)
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
int CPUOpcodes::op_9B(CPURegisters * registers)
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
int CPUOpcodes::op_9C(CPURegisters * registers)
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
int CPUOpcodes::op_9D(CPURegisters * registers)
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
int CPUOpcodes::op_9E(CPURegisters * registers)
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
int CPUOpcodes::op_9F(CPURegisters * registers)
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