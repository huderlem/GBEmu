#pragma once

#include "CPURegisters.h"
#include "IMMU.h"

#define TEST_API __declspec(dllexport)

class TEST_API CPUOpcodes
{
public:
	CPUOpcodes(IMMU *mmu);
	~CPUOpcodes();
	IMMU *mmu;
	int op_00(CPURegisters *registers);
	int op_01(CPURegisters *registers);
	int op_02(CPURegisters *registers);
	int op_03(CPURegisters *registers);
	int op_04(CPURegisters *registers);
	int op_05(CPURegisters *registers);
	int op_06(CPURegisters *registers);
	int op_07(CPURegisters *registers);
	int op_08(CPURegisters *registers);
	int op_09(CPURegisters *registers);
	int op_0A(CPURegisters *registers);
	int op_0B(CPURegisters *registers);
	int op_0C(CPURegisters *registers);
	int op_0D(CPURegisters *registers);
	int op_0E(CPURegisters *registers);
	int op_0F(CPURegisters *registers);
};


