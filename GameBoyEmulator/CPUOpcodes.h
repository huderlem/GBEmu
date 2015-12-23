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
	int op_40(CPURegisters *registers);
	int op_41(CPURegisters *registers);
	int op_42(CPURegisters *registers);
	int op_43(CPURegisters *registers);
	int op_44(CPURegisters *registers);
	int op_45(CPURegisters *registers);
	int op_46(CPURegisters *registers);
	int op_47(CPURegisters *registers);
	int op_48(CPURegisters *registers);
	int op_49(CPURegisters *registers);
	int op_4A(CPURegisters *registers);
	int op_4B(CPURegisters *registers);
	int op_4C(CPURegisters *registers);
	int op_4D(CPURegisters *registers);
	int op_4E(CPURegisters *registers);
	int op_4F(CPURegisters *registers);
	int op_50(CPURegisters *registers);
	int op_51(CPURegisters *registers);
	int op_52(CPURegisters *registers);
	int op_53(CPURegisters *registers);
	int op_54(CPURegisters *registers);
	int op_55(CPURegisters *registers);
	int op_56(CPURegisters *registers);
	int op_57(CPURegisters *registers);
	int op_58(CPURegisters *registers);
	int op_59(CPURegisters *registers);
	int op_5A(CPURegisters *registers);
	int op_5B(CPURegisters *registers);
	int op_5C(CPURegisters *registers);
	int op_5D(CPURegisters *registers);
	int op_5E(CPURegisters *registers);
	int op_5F(CPURegisters *registers);
	int op_60(CPURegisters *registers);
	int op_61(CPURegisters *registers);
	int op_62(CPURegisters *registers);
	int op_63(CPURegisters *registers);
	int op_64(CPURegisters *registers);
	int op_65(CPURegisters *registers);
	int op_66(CPURegisters *registers);
	int op_67(CPURegisters *registers);
	int op_68(CPURegisters *registers);
	int op_69(CPURegisters *registers);
	int op_6A(CPURegisters *registers);
	int op_6B(CPURegisters *registers);
	int op_6C(CPURegisters *registers);
	int op_6D(CPURegisters *registers);
	int op_6E(CPURegisters *registers);
	int op_6F(CPURegisters *registers);
};


