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
	int op_70(CPURegisters *registers);
	int op_71(CPURegisters *registers);
	int op_72(CPURegisters *registers);
	int op_73(CPURegisters *registers);
	int op_74(CPURegisters *registers);
	int op_75(CPURegisters *registers);
	int op_76(CPURegisters *registers);
	int op_77(CPURegisters *registers);
	int op_78(CPURegisters *registers);
	int op_79(CPURegisters *registers);
	int op_7A(CPURegisters *registers);
	int op_7B(CPURegisters *registers);
	int op_7C(CPURegisters *registers);
	int op_7D(CPURegisters *registers);
	int op_7E(CPURegisters *registers);
	int op_7F(CPURegisters *registers);
	int op_80(CPURegisters *registers);
	int op_81(CPURegisters *registers);
	int op_82(CPURegisters *registers);
	int op_83(CPURegisters *registers);
	int op_84(CPURegisters *registers);
	int op_85(CPURegisters *registers);
	int op_86(CPURegisters *registers);
	int op_87(CPURegisters *registers);
	int op_88(CPURegisters *registers);
	int op_89(CPURegisters *registers);
	int op_8A(CPURegisters *registers);
	int op_8B(CPURegisters *registers);
	int op_8C(CPURegisters *registers);
	int op_8D(CPURegisters *registers);
	int op_8E(CPURegisters *registers);
	int op_8F(CPURegisters *registers);
	int op_90(CPURegisters *registers);
	int op_91(CPURegisters *registers);
	int op_92(CPURegisters *registers);
	int op_93(CPURegisters *registers);
	int op_94(CPURegisters *registers);
	int op_95(CPURegisters *registers);
	int op_96(CPURegisters *registers);
	int op_97(CPURegisters *registers);
	int op_98(CPURegisters *registers);
	int op_99(CPURegisters *registers);
	int op_9A(CPURegisters *registers);
	int op_9B(CPURegisters *registers);
	int op_9C(CPURegisters *registers);
	int op_9D(CPURegisters *registers);
	int op_9E(CPURegisters *registers);
	int op_9F(CPURegisters *registers);
	int op_A0(CPURegisters *registers);
	int op_A1(CPURegisters *registers);
	int op_A2(CPURegisters *registers);
	int op_A3(CPURegisters *registers);
	int op_A4(CPURegisters *registers);
	int op_A5(CPURegisters *registers);
	int op_A6(CPURegisters *registers);
	int op_A7(CPURegisters *registers);
	int op_A8(CPURegisters *registers);
	int op_A9(CPURegisters *registers);
	int op_AA(CPURegisters *registers);
	int op_AB(CPURegisters *registers);
	int op_AC(CPURegisters *registers);
	int op_AD(CPURegisters *registers);
	int op_AE(CPURegisters *registers);
	int op_AF(CPURegisters *registers);
	int op_B0(CPURegisters *registers);
	int op_B1(CPURegisters *registers);
	int op_B2(CPURegisters *registers);
	int op_B3(CPURegisters *registers);
	int op_B4(CPURegisters *registers);
	int op_B5(CPURegisters *registers);
	int op_B6(CPURegisters *registers);
	int op_B7(CPURegisters *registers);
	int op_B8(CPURegisters *registers);
	int op_B9(CPURegisters *registers);
	int op_BA(CPURegisters *registers);
	int op_BB(CPURegisters *registers);
	int op_BC(CPURegisters *registers);
	int op_BD(CPURegisters *registers);
	int op_BE(CPURegisters *registers);
	int op_BF(CPURegisters *registers);
	int op_C0(CPURegisters *registers);
	int op_C1(CPURegisters *registers);
	int op_C2(CPURegisters *registers);
	int op_C3(CPURegisters *registers);
	int op_C4(CPURegisters *registers);
	int op_C5(CPURegisters *registers);
	int op_C6(CPURegisters *registers);
	int op_C7(CPURegisters *registers);
	int op_C8(CPURegisters *registers);
	int op_C9(CPURegisters *registers);
	int op_CA(CPURegisters *registers);
	int op_CB(CPURegisters *registers);
	int op_CC(CPURegisters *registers);
	int op_CD(CPURegisters *registers);
	int op_CE(CPURegisters *registers);
	int op_CF(CPURegisters *registers);
};


