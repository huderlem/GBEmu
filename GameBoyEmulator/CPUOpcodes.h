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
	int op_10(CPURegisters *registers);
	int op_11(CPURegisters *registers);
	int op_12(CPURegisters *registers);
	int op_13(CPURegisters *registers);
	int op_14(CPURegisters *registers);
	int op_15(CPURegisters *registers);
	int op_16(CPURegisters *registers);
	int op_17(CPURegisters *registers);
	int op_18(CPURegisters *registers);
	int op_19(CPURegisters *registers);
	int op_1A(CPURegisters *registers);
	int op_1B(CPURegisters *registers);
	int op_1C(CPURegisters *registers);
	int op_1D(CPURegisters *registers);
	int op_1E(CPURegisters *registers);
	int op_1F(CPURegisters *registers);
	int op_20(CPURegisters *registers);
	int op_21(CPURegisters *registers);
	int op_22(CPURegisters *registers);
	int op_23(CPURegisters *registers);
	int op_24(CPURegisters *registers);
	int op_25(CPURegisters *registers);
	int op_26(CPURegisters *registers);
	int op_27(CPURegisters *registers);
	int op_28(CPURegisters *registers);
	int op_29(CPURegisters *registers);
	int op_2A(CPURegisters *registers);
	int op_2B(CPURegisters *registers);
	int op_2C(CPURegisters *registers);
	int op_2D(CPURegisters *registers);
	int op_2E(CPURegisters *registers);
	int op_2F(CPURegisters *registers);
	int op_30(CPURegisters *registers);
	int op_31(CPURegisters *registers);
	int op_32(CPURegisters *registers);
	int op_33(CPURegisters *registers);
	int op_34(CPURegisters *registers);
	int op_35(CPURegisters *registers);
	int op_36(CPURegisters *registers);
	int op_37(CPURegisters *registers);
	int op_38(CPURegisters *registers);
	int op_39(CPURegisters *registers);
	int op_3A(CPURegisters *registers);
	int op_3B(CPURegisters *registers);
	int op_3C(CPURegisters *registers);
	int op_3D(CPURegisters *registers);
	int op_3E(CPURegisters *registers);
	int op_3F(CPURegisters *registers);
};


