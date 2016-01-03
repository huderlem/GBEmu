#pragma once

#include <stdexcept>

#include "CPURegisters.h"
#include "IMMU.h"
#include "Interrupts.h"

#define TEST_API __declspec(dllexport)

class TEST_API CPUOpcodes
{
public:
	CPUOpcodes(IMMU *mmu, Interrupts *interrupts);
	~CPUOpcodes();
	int ExecuteInstruction(int opcode, CPURegisters *registers);

	typedef int (CPUOpcodes::*Instruction_Func)(CPURegisters *);

	IMMU *mmu;
	Interrupts *interrupts;
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
	int op_D0(CPURegisters *registers);
	int op_D1(CPURegisters *registers);
	int op_D2(CPURegisters *registers);
	int op_D4(CPURegisters *registers);
	int op_D5(CPURegisters *registers);
	int op_D6(CPURegisters *registers);
	int op_D7(CPURegisters *registers);
	int op_D8(CPURegisters *registers);
	int op_D9(CPURegisters *registers);
	int op_DA(CPURegisters *registers);
	int op_DC(CPURegisters *registers);
	int op_DE(CPURegisters *registers);
	int op_DF(CPURegisters *registers);
	int op_E0(CPURegisters *registers);
	int op_E1(CPURegisters *registers);
	int op_E2(CPURegisters *registers);
	int op_E5(CPURegisters *registers);
	int op_E6(CPURegisters *registers);
	int op_E7(CPURegisters *registers);
	int op_E8(CPURegisters *registers);
	int op_E9(CPURegisters *registers);
	int op_EA(CPURegisters *registers);
	int op_EE(CPURegisters *registers);
	int op_EF(CPURegisters *registers);
	int op_F0(CPURegisters *registers);
	int op_F1(CPURegisters *registers);
	int op_F2(CPURegisters *registers);
	int op_F3(CPURegisters *registers);
	int op_F5(CPURegisters *registers);
	int op_F6(CPURegisters *registers);
	int op_F7(CPURegisters *registers);
	int op_F8(CPURegisters *registers);
	int op_F9(CPURegisters *registers);
	int op_FA(CPURegisters *registers);
	int op_FB(CPURegisters *registers);
	int op_FE(CPURegisters *registers);
	int op_FF(CPURegisters *registers);

	void op_CB_RLC(CPURegisters *registers, int *value);
	int op_CB00(CPURegisters *registers);
	int op_CB01(CPURegisters *registers);
	int op_CB02(CPURegisters *registers);
	int op_CB03(CPURegisters *registers);
	int op_CB04(CPURegisters *registers);
	int op_CB05(CPURegisters *registers);
	int op_CB06(CPURegisters *registers);
	int op_CB07(CPURegisters *registers);

	void op_CB_RRC(CPURegisters *registers, int *value);
	int op_CB08(CPURegisters *registers);
	int op_CB09(CPURegisters *registers);
	int op_CB0A(CPURegisters *registers);
	int op_CB0B(CPURegisters *registers);
	int op_CB0C(CPURegisters *registers);
	int op_CB0D(CPURegisters *registers);
	int op_CB0E(CPURegisters *registers);
	int op_CB0F(CPURegisters *registers);

	void op_CB_RL(CPURegisters *registers, int *value);
	int op_CB10(CPURegisters *registers);
	int op_CB11(CPURegisters *registers);
	int op_CB12(CPURegisters *registers);
	int op_CB13(CPURegisters *registers);
	int op_CB14(CPURegisters *registers);
	int op_CB15(CPURegisters *registers);
	int op_CB16(CPURegisters *registers);
	int op_CB17(CPURegisters *registers);

	void op_CB_RR(CPURegisters *registers, int *value);
	int op_CB18(CPURegisters *registers);
	int op_CB19(CPURegisters *registers);
	int op_CB1A(CPURegisters *registers);
	int op_CB1B(CPURegisters *registers);
	int op_CB1C(CPURegisters *registers);
	int op_CB1D(CPURegisters *registers);
	int op_CB1E(CPURegisters *registers);
	int op_CB1F(CPURegisters *registers);

	void op_CB_SLA(CPURegisters *registers, int *value);
	int op_CB20(CPURegisters *registers);
	int op_CB21(CPURegisters *registers);
	int op_CB22(CPURegisters *registers);
	int op_CB23(CPURegisters *registers);
	int op_CB24(CPURegisters *registers);
	int op_CB25(CPURegisters *registers);
	int op_CB26(CPURegisters *registers);
	int op_CB27(CPURegisters *registers);

	void op_CB_SRA(CPURegisters *registers, int *value);
	int op_CB28(CPURegisters *registers);
	int op_CB29(CPURegisters *registers);
	int op_CB2A(CPURegisters *registers);
	int op_CB2B(CPURegisters *registers);
	int op_CB2C(CPURegisters *registers);
	int op_CB2D(CPURegisters *registers);
	int op_CB2E(CPURegisters *registers);
	int op_CB2F(CPURegisters *registers);

	void op_CB_SWAP(CPURegisters *registers, int *value);
	int op_CB30(CPURegisters *registers);
	int op_CB31(CPURegisters *registers);
	int op_CB32(CPURegisters *registers);
	int op_CB33(CPURegisters *registers);
	int op_CB34(CPURegisters *registers);
	int op_CB35(CPURegisters *registers);
	int op_CB36(CPURegisters *registers);
	int op_CB37(CPURegisters *registers);

	void op_CB_SRL(CPURegisters *registers, int *value);
	int op_CB38(CPURegisters *registers);
	int op_CB39(CPURegisters *registers);
	int op_CB3A(CPURegisters *registers);
	int op_CB3B(CPURegisters *registers);
	int op_CB3C(CPURegisters *registers);
	int op_CB3D(CPURegisters *registers);
	int op_CB3E(CPURegisters *registers);
	int op_CB3F(CPURegisters *registers);

	void op_CB_BIT(CPURegisters *registers, int bitIndex, int *value);
	int op_CB40(CPURegisters *registers);
	int op_CB41(CPURegisters *registers);
	int op_CB42(CPURegisters *registers);
	int op_CB43(CPURegisters *registers);
	int op_CB44(CPURegisters *registers);
	int op_CB45(CPURegisters *registers);
	int op_CB46(CPURegisters *registers);
	int op_CB47(CPURegisters *registers);
	int op_CB48(CPURegisters *registers);
	int op_CB49(CPURegisters *registers);
	int op_CB4A(CPURegisters *registers);
	int op_CB4B(CPURegisters *registers);
	int op_CB4C(CPURegisters *registers);
	int op_CB4D(CPURegisters *registers);
	int op_CB4E(CPURegisters *registers);
	int op_CB4F(CPURegisters *registers);
	int op_CB50(CPURegisters *registers);
	int op_CB51(CPURegisters *registers);
	int op_CB52(CPURegisters *registers);
	int op_CB53(CPURegisters *registers);
	int op_CB54(CPURegisters *registers);
	int op_CB55(CPURegisters *registers);
	int op_CB56(CPURegisters *registers);
	int op_CB57(CPURegisters *registers);
	int op_CB58(CPURegisters *registers);
	int op_CB59(CPURegisters *registers);
	int op_CB5A(CPURegisters *registers);
	int op_CB5B(CPURegisters *registers);
	int op_CB5C(CPURegisters *registers);
	int op_CB5D(CPURegisters *registers);
	int op_CB5E(CPURegisters *registers);
	int op_CB5F(CPURegisters *registers);
	int op_CB60(CPURegisters *registers);
	int op_CB61(CPURegisters *registers);
	int op_CB62(CPURegisters *registers);
	int op_CB63(CPURegisters *registers);
	int op_CB64(CPURegisters *registers);
	int op_CB65(CPURegisters *registers);
	int op_CB66(CPURegisters *registers);
	int op_CB67(CPURegisters *registers);
	int op_CB68(CPURegisters *registers);
	int op_CB69(CPURegisters *registers);
	int op_CB6A(CPURegisters *registers);
	int op_CB6B(CPURegisters *registers);
	int op_CB6C(CPURegisters *registers);
	int op_CB6D(CPURegisters *registers);
	int op_CB6E(CPURegisters *registers);
	int op_CB6F(CPURegisters *registers);
	int op_CB70(CPURegisters *registers);
	int op_CB71(CPURegisters *registers);
	int op_CB72(CPURegisters *registers);
	int op_CB73(CPURegisters *registers);
	int op_CB74(CPURegisters *registers);
	int op_CB75(CPURegisters *registers);
	int op_CB76(CPURegisters *registers);
	int op_CB77(CPURegisters *registers);
	int op_CB78(CPURegisters *registers);
	int op_CB79(CPURegisters *registers);
	int op_CB7A(CPURegisters *registers);
	int op_CB7B(CPURegisters *registers);
	int op_CB7C(CPURegisters *registers);
	int op_CB7D(CPURegisters *registers);
	int op_CB7E(CPURegisters *registers);
	int op_CB7F(CPURegisters *registers);

	void op_CB_RES(CPURegisters *registers, int bitIndex, int *value);
	int op_CB80(CPURegisters *registers);
	int op_CB81(CPURegisters *registers);
	int op_CB82(CPURegisters *registers);
	int op_CB83(CPURegisters *registers);
	int op_CB84(CPURegisters *registers);
	int op_CB85(CPURegisters *registers);
	int op_CB86(CPURegisters *registers);
	int op_CB87(CPURegisters *registers);
	int op_CB88(CPURegisters *registers);
	int op_CB89(CPURegisters *registers);
	int op_CB8A(CPURegisters *registers);
	int op_CB8B(CPURegisters *registers);
	int op_CB8C(CPURegisters *registers);
	int op_CB8D(CPURegisters *registers);
	int op_CB8E(CPURegisters *registers);
	int op_CB8F(CPURegisters *registers);
	int op_CB90(CPURegisters *registers);
	int op_CB91(CPURegisters *registers);
	int op_CB92(CPURegisters *registers);
	int op_CB93(CPURegisters *registers);
	int op_CB94(CPURegisters *registers);
	int op_CB95(CPURegisters *registers);
	int op_CB96(CPURegisters *registers);
	int op_CB97(CPURegisters *registers);
	int op_CB98(CPURegisters *registers);
	int op_CB99(CPURegisters *registers);
	int op_CB9A(CPURegisters *registers);
	int op_CB9B(CPURegisters *registers);
	int op_CB9C(CPURegisters *registers);
	int op_CB9D(CPURegisters *registers);
	int op_CB9E(CPURegisters *registers);
	int op_CB9F(CPURegisters *registers);
	int op_CBA0(CPURegisters *registers);
	int op_CBA1(CPURegisters *registers);
	int op_CBA2(CPURegisters *registers);
	int op_CBA3(CPURegisters *registers);
	int op_CBA4(CPURegisters *registers);
	int op_CBA5(CPURegisters *registers);
	int op_CBA6(CPURegisters *registers);
	int op_CBA7(CPURegisters *registers);
	int op_CBA8(CPURegisters *registers);
	int op_CBA9(CPURegisters *registers);
	int op_CBAA(CPURegisters *registers);
	int op_CBAB(CPURegisters *registers);
	int op_CBAC(CPURegisters *registers);
	int op_CBAD(CPURegisters *registers);
	int op_CBAE(CPURegisters *registers);
	int op_CBAF(CPURegisters *registers);
	int op_CBB0(CPURegisters *registers);
	int op_CBB1(CPURegisters *registers);
	int op_CBB2(CPURegisters *registers);
	int op_CBB3(CPURegisters *registers);
	int op_CBB4(CPURegisters *registers);
	int op_CBB5(CPURegisters *registers);
	int op_CBB6(CPURegisters *registers);
	int op_CBB7(CPURegisters *registers);
	int op_CBB8(CPURegisters *registers);
	int op_CBB9(CPURegisters *registers);
	int op_CBBA(CPURegisters *registers);
	int op_CBBB(CPURegisters *registers);
	int op_CBBC(CPURegisters *registers);
	int op_CBBD(CPURegisters *registers);
	int op_CBBE(CPURegisters *registers);
	int op_CBBF(CPURegisters *registers);

	void op_CB_SET(CPURegisters *registers, int bitIndex, int *value);
	int op_CBC0(CPURegisters *registers);
	int op_CBC1(CPURegisters *registers);
	int op_CBC2(CPURegisters *registers);
	int op_CBC3(CPURegisters *registers);
	int op_CBC4(CPURegisters *registers);
	int op_CBC5(CPURegisters *registers);
	int op_CBC6(CPURegisters *registers);
	int op_CBC7(CPURegisters *registers);
	int op_CBC8(CPURegisters *registers);
	int op_CBC9(CPURegisters *registers);
	int op_CBCA(CPURegisters *registers);
	int op_CBCB(CPURegisters *registers);
	int op_CBCC(CPURegisters *registers);
	int op_CBCD(CPURegisters *registers);
	int op_CBCE(CPURegisters *registers);
	int op_CBCF(CPURegisters *registers);
	int op_CBD0(CPURegisters *registers);
	int op_CBD1(CPURegisters *registers);
	int op_CBD2(CPURegisters *registers);
	int op_CBD3(CPURegisters *registers);
	int op_CBD4(CPURegisters *registers);
	int op_CBD5(CPURegisters *registers);
	int op_CBD6(CPURegisters *registers);
	int op_CBD7(CPURegisters *registers);
	int op_CBD8(CPURegisters *registers);
	int op_CBD9(CPURegisters *registers);
	int op_CBDA(CPURegisters *registers);
	int op_CBDB(CPURegisters *registers);
	int op_CBDC(CPURegisters *registers);
	int op_CBDD(CPURegisters *registers);
	int op_CBDE(CPURegisters *registers);
	int op_CBDF(CPURegisters *registers);
	int op_CBE0(CPURegisters *registers);
	int op_CBE1(CPURegisters *registers);
	int op_CBE2(CPURegisters *registers);
	int op_CBE3(CPURegisters *registers);
	int op_CBE4(CPURegisters *registers);
	int op_CBE5(CPURegisters *registers);
	int op_CBE6(CPURegisters *registers);
	int op_CBE7(CPURegisters *registers);
	int op_CBE8(CPURegisters *registers);
	int op_CBE9(CPURegisters *registers);
	int op_CBEA(CPURegisters *registers);
	int op_CBEB(CPURegisters *registers);
	int op_CBEC(CPURegisters *registers);
	int op_CBED(CPURegisters *registers);
	int op_CBEE(CPURegisters *registers);
	int op_CBEF(CPURegisters *registers);
	int op_CBF0(CPURegisters *registers);
	int op_CBF1(CPURegisters *registers);
	int op_CBF2(CPURegisters *registers);
	int op_CBF3(CPURegisters *registers);
	int op_CBF4(CPURegisters *registers);
	int op_CBF5(CPURegisters *registers);
	int op_CBF6(CPURegisters *registers);
	int op_CBF7(CPURegisters *registers);
	int op_CBF8(CPURegisters *registers);
	int op_CBF9(CPURegisters *registers);
	int op_CBFA(CPURegisters *registers);
	int op_CBFB(CPURegisters *registers);
	int op_CBFC(CPURegisters *registers);
	int op_CBFD(CPURegisters *registers);
	int op_CBFE(CPURegisters *registers);
	int op_CBFF(CPURegisters *registers);

	Instruction_Func Instructions[0x100] =
	{
		&CPUOpcodes::op_00, &CPUOpcodes::op_01,
		&CPUOpcodes::op_02, &CPUOpcodes::op_03,
		&CPUOpcodes::op_04, &CPUOpcodes::op_05,
		&CPUOpcodes::op_06, &CPUOpcodes::op_07,
		&CPUOpcodes::op_08, &CPUOpcodes::op_09,
		&CPUOpcodes::op_0A, &CPUOpcodes::op_0B,
		&CPUOpcodes::op_0C, &CPUOpcodes::op_0D,
		&CPUOpcodes::op_0E, &CPUOpcodes::op_0F,
		&CPUOpcodes::op_10, &CPUOpcodes::op_11,
		&CPUOpcodes::op_12, &CPUOpcodes::op_13,
		&CPUOpcodes::op_14, &CPUOpcodes::op_15,
		&CPUOpcodes::op_16, &CPUOpcodes::op_17,
		&CPUOpcodes::op_18, &CPUOpcodes::op_19,
		&CPUOpcodes::op_1A, &CPUOpcodes::op_1B,
		&CPUOpcodes::op_1C, &CPUOpcodes::op_1D,
		&CPUOpcodes::op_1E, &CPUOpcodes::op_1F,
		&CPUOpcodes::op_20, &CPUOpcodes::op_21,
		&CPUOpcodes::op_22, &CPUOpcodes::op_23,
		&CPUOpcodes::op_24, &CPUOpcodes::op_25,
		&CPUOpcodes::op_26, &CPUOpcodes::op_27,
		&CPUOpcodes::op_28, &CPUOpcodes::op_29,
		&CPUOpcodes::op_2A, &CPUOpcodes::op_2B,
		&CPUOpcodes::op_2C, &CPUOpcodes::op_2D,
		&CPUOpcodes::op_2E, &CPUOpcodes::op_2F,
		&CPUOpcodes::op_30, &CPUOpcodes::op_31,
		&CPUOpcodes::op_32, &CPUOpcodes::op_33,
		&CPUOpcodes::op_34, &CPUOpcodes::op_35,
		&CPUOpcodes::op_36, &CPUOpcodes::op_37,
		&CPUOpcodes::op_38, &CPUOpcodes::op_39,
		&CPUOpcodes::op_3A, &CPUOpcodes::op_3B,
		&CPUOpcodes::op_3C, &CPUOpcodes::op_3D,
		&CPUOpcodes::op_3E, &CPUOpcodes::op_3F,
		&CPUOpcodes::op_40, &CPUOpcodes::op_41,
		&CPUOpcodes::op_42, &CPUOpcodes::op_43,
		&CPUOpcodes::op_44, &CPUOpcodes::op_45,
		&CPUOpcodes::op_46, &CPUOpcodes::op_47,
		&CPUOpcodes::op_48, &CPUOpcodes::op_49,
		&CPUOpcodes::op_4A, &CPUOpcodes::op_4B,
		&CPUOpcodes::op_4C, &CPUOpcodes::op_4D,
		&CPUOpcodes::op_4E, &CPUOpcodes::op_4F,
		&CPUOpcodes::op_50, &CPUOpcodes::op_51,
		&CPUOpcodes::op_52, &CPUOpcodes::op_53,
		&CPUOpcodes::op_54, &CPUOpcodes::op_55,
		&CPUOpcodes::op_56, &CPUOpcodes::op_57,
		&CPUOpcodes::op_58, &CPUOpcodes::op_59,
		&CPUOpcodes::op_5A, &CPUOpcodes::op_5B,
		&CPUOpcodes::op_5C, &CPUOpcodes::op_5D,
		&CPUOpcodes::op_5E, &CPUOpcodes::op_5F,
		&CPUOpcodes::op_60, &CPUOpcodes::op_61,
		&CPUOpcodes::op_62, &CPUOpcodes::op_63,
		&CPUOpcodes::op_64, &CPUOpcodes::op_65,
		&CPUOpcodes::op_66, &CPUOpcodes::op_67,
		&CPUOpcodes::op_68, &CPUOpcodes::op_69,
		&CPUOpcodes::op_6A, &CPUOpcodes::op_6B,
		&CPUOpcodes::op_6C, &CPUOpcodes::op_6D,
		&CPUOpcodes::op_6E, &CPUOpcodes::op_6F,
		&CPUOpcodes::op_70, &CPUOpcodes::op_71,
		&CPUOpcodes::op_72, &CPUOpcodes::op_73,
		&CPUOpcodes::op_74, &CPUOpcodes::op_75,
		&CPUOpcodes::op_76, &CPUOpcodes::op_77,
		&CPUOpcodes::op_78, &CPUOpcodes::op_79,
		&CPUOpcodes::op_7A, &CPUOpcodes::op_7B,
		&CPUOpcodes::op_7C, &CPUOpcodes::op_7D,
		&CPUOpcodes::op_7E, &CPUOpcodes::op_7F,
		&CPUOpcodes::op_80, &CPUOpcodes::op_81,
		&CPUOpcodes::op_82, &CPUOpcodes::op_83,
		&CPUOpcodes::op_84, &CPUOpcodes::op_85,
		&CPUOpcodes::op_86, &CPUOpcodes::op_87,
		&CPUOpcodes::op_88, &CPUOpcodes::op_89,
		&CPUOpcodes::op_8A, &CPUOpcodes::op_8B,
		&CPUOpcodes::op_8C, &CPUOpcodes::op_8D,
		&CPUOpcodes::op_8E, &CPUOpcodes::op_8F,
		&CPUOpcodes::op_90, &CPUOpcodes::op_91,
		&CPUOpcodes::op_92, &CPUOpcodes::op_93,
		&CPUOpcodes::op_94, &CPUOpcodes::op_95,
		&CPUOpcodes::op_96, &CPUOpcodes::op_97,
		&CPUOpcodes::op_98, &CPUOpcodes::op_99,
		&CPUOpcodes::op_9A, &CPUOpcodes::op_9B,
		&CPUOpcodes::op_9C, &CPUOpcodes::op_9D,
		&CPUOpcodes::op_9E, &CPUOpcodes::op_9F,
		&CPUOpcodes::op_A0, &CPUOpcodes::op_A1,
		&CPUOpcodes::op_A2, &CPUOpcodes::op_A3,
		&CPUOpcodes::op_A4, &CPUOpcodes::op_A5,
		&CPUOpcodes::op_A6, &CPUOpcodes::op_A7,
		&CPUOpcodes::op_A8, &CPUOpcodes::op_A9,
		&CPUOpcodes::op_AA, &CPUOpcodes::op_AB,
		&CPUOpcodes::op_AC, &CPUOpcodes::op_AD,
		&CPUOpcodes::op_AE, &CPUOpcodes::op_AF,
		&CPUOpcodes::op_B0, &CPUOpcodes::op_B1,
		&CPUOpcodes::op_B2, &CPUOpcodes::op_B3,
		&CPUOpcodes::op_B4, &CPUOpcodes::op_B5,
		&CPUOpcodes::op_B6, &CPUOpcodes::op_B7,
		&CPUOpcodes::op_B8, &CPUOpcodes::op_B9,
		&CPUOpcodes::op_BA, &CPUOpcodes::op_BB,
		&CPUOpcodes::op_BC, &CPUOpcodes::op_BD,
		&CPUOpcodes::op_BE, &CPUOpcodes::op_BF,
		&CPUOpcodes::op_C0, &CPUOpcodes::op_C1,
		&CPUOpcodes::op_C2, &CPUOpcodes::op_C3,
		&CPUOpcodes::op_C4, &CPUOpcodes::op_C5,
		&CPUOpcodes::op_C6, &CPUOpcodes::op_C7,
		&CPUOpcodes::op_C8, &CPUOpcodes::op_C9,
		&CPUOpcodes::op_CA, &CPUOpcodes::op_CB,
		&CPUOpcodes::op_CC, &CPUOpcodes::op_CD,
		&CPUOpcodes::op_CE, &CPUOpcodes::op_CF,
		&CPUOpcodes::op_D0, &CPUOpcodes::op_D1,
		&CPUOpcodes::op_D2, nullptr,
		&CPUOpcodes::op_D4, &CPUOpcodes::op_D5,
		&CPUOpcodes::op_D6, &CPUOpcodes::op_D7,
		&CPUOpcodes::op_D8, &CPUOpcodes::op_D9,
		&CPUOpcodes::op_DA, nullptr,
		&CPUOpcodes::op_DC, nullptr,
		&CPUOpcodes::op_DE, &CPUOpcodes::op_DF,
		&CPUOpcodes::op_E0, &CPUOpcodes::op_E1,
		&CPUOpcodes::op_E2, nullptr,
		nullptr,            &CPUOpcodes::op_E5,
		&CPUOpcodes::op_E6, &CPUOpcodes::op_E7,
		&CPUOpcodes::op_E8, &CPUOpcodes::op_E9,
		&CPUOpcodes::op_EA, nullptr,
		nullptr,            nullptr,
		&CPUOpcodes::op_EE, &CPUOpcodes::op_EF,
		&CPUOpcodes::op_F0, &CPUOpcodes::op_F1,
		&CPUOpcodes::op_F2, &CPUOpcodes::op_F3,
		nullptr,            &CPUOpcodes::op_F5,
		&CPUOpcodes::op_F6, &CPUOpcodes::op_F7,
		&CPUOpcodes::op_F8, &CPUOpcodes::op_F9,
		&CPUOpcodes::op_FA, &CPUOpcodes::op_FB,
		nullptr,            nullptr,
		&CPUOpcodes::op_FE, &CPUOpcodes::op_FF,
	};

	Instruction_Func CB_Instructions[0x100] =
	{
		&CPUOpcodes::op_CB00, &CPUOpcodes::op_CB01,
		&CPUOpcodes::op_CB02, &CPUOpcodes::op_CB03,
		&CPUOpcodes::op_CB04, &CPUOpcodes::op_CB05,
		&CPUOpcodes::op_CB06, &CPUOpcodes::op_CB07,
		&CPUOpcodes::op_CB08, &CPUOpcodes::op_CB09,
		&CPUOpcodes::op_CB0A, &CPUOpcodes::op_CB0B,
		&CPUOpcodes::op_CB0C, &CPUOpcodes::op_CB0D,
		&CPUOpcodes::op_CB0E, &CPUOpcodes::op_CB0F,
		&CPUOpcodes::op_CB10, &CPUOpcodes::op_CB11,
		&CPUOpcodes::op_CB12, &CPUOpcodes::op_CB13,
		&CPUOpcodes::op_CB14, &CPUOpcodes::op_CB15,
		&CPUOpcodes::op_CB16, &CPUOpcodes::op_CB17,
		&CPUOpcodes::op_CB18, &CPUOpcodes::op_CB19,
		&CPUOpcodes::op_CB1A, &CPUOpcodes::op_CB1B,
		&CPUOpcodes::op_CB1C, &CPUOpcodes::op_CB1D,
		&CPUOpcodes::op_CB1E, &CPUOpcodes::op_CB1F,
		&CPUOpcodes::op_CB20, &CPUOpcodes::op_CB21,
		&CPUOpcodes::op_CB22, &CPUOpcodes::op_CB23,
		&CPUOpcodes::op_CB24, &CPUOpcodes::op_CB25,
		&CPUOpcodes::op_CB26, &CPUOpcodes::op_CB27,
		&CPUOpcodes::op_CB28, &CPUOpcodes::op_CB29,
		&CPUOpcodes::op_CB2A, &CPUOpcodes::op_CB2B,
		&CPUOpcodes::op_CB2C, &CPUOpcodes::op_CB2D,
		&CPUOpcodes::op_CB2E, &CPUOpcodes::op_CB2F,
		&CPUOpcodes::op_CB30, &CPUOpcodes::op_CB31,
		&CPUOpcodes::op_CB32, &CPUOpcodes::op_CB33,
		&CPUOpcodes::op_CB34, &CPUOpcodes::op_CB35,
		&CPUOpcodes::op_CB36, &CPUOpcodes::op_CB37,
		&CPUOpcodes::op_CB38, &CPUOpcodes::op_CB39,
		&CPUOpcodes::op_CB3A, &CPUOpcodes::op_CB3B,
		&CPUOpcodes::op_CB3C, &CPUOpcodes::op_CB3D,
		&CPUOpcodes::op_CB3E, &CPUOpcodes::op_CB3F,
		&CPUOpcodes::op_CB40, &CPUOpcodes::op_CB41,
		&CPUOpcodes::op_CB42, &CPUOpcodes::op_CB43,
		&CPUOpcodes::op_CB44, &CPUOpcodes::op_CB45,
		&CPUOpcodes::op_CB46, &CPUOpcodes::op_CB47,
		&CPUOpcodes::op_CB48, &CPUOpcodes::op_CB49,
		&CPUOpcodes::op_CB4A, &CPUOpcodes::op_CB4B,
		&CPUOpcodes::op_CB4C, &CPUOpcodes::op_CB4D,
		&CPUOpcodes::op_CB4E, &CPUOpcodes::op_CB4F,
		&CPUOpcodes::op_CB50, &CPUOpcodes::op_CB51,
		&CPUOpcodes::op_CB52, &CPUOpcodes::op_CB53,
		&CPUOpcodes::op_CB54, &CPUOpcodes::op_CB55,
		&CPUOpcodes::op_CB56, &CPUOpcodes::op_CB57,
		&CPUOpcodes::op_CB58, &CPUOpcodes::op_CB59,
		&CPUOpcodes::op_CB5A, &CPUOpcodes::op_CB5B,
		&CPUOpcodes::op_CB5C, &CPUOpcodes::op_CB5D,
		&CPUOpcodes::op_CB5E, &CPUOpcodes::op_CB5F,
		&CPUOpcodes::op_CB60, &CPUOpcodes::op_CB61,
		&CPUOpcodes::op_CB62, &CPUOpcodes::op_CB63,
		&CPUOpcodes::op_CB64, &CPUOpcodes::op_CB65,
		&CPUOpcodes::op_CB66, &CPUOpcodes::op_CB67,
		&CPUOpcodes::op_CB68, &CPUOpcodes::op_CB69,
		&CPUOpcodes::op_CB6A, &CPUOpcodes::op_CB6B,
		&CPUOpcodes::op_CB6C, &CPUOpcodes::op_CB6D,
		&CPUOpcodes::op_CB6E, &CPUOpcodes::op_CB6F,
		&CPUOpcodes::op_CB70, &CPUOpcodes::op_CB71,
		&CPUOpcodes::op_CB72, &CPUOpcodes::op_CB73,
		&CPUOpcodes::op_CB74, &CPUOpcodes::op_CB75,
		&CPUOpcodes::op_CB76, &CPUOpcodes::op_CB77,
		&CPUOpcodes::op_CB78, &CPUOpcodes::op_CB79,
		&CPUOpcodes::op_CB7A, &CPUOpcodes::op_CB7B,
		&CPUOpcodes::op_CB7C, &CPUOpcodes::op_CB7D,
		&CPUOpcodes::op_CB7E, &CPUOpcodes::op_CB7F,
		&CPUOpcodes::op_CB80, &CPUOpcodes::op_CB81,
		&CPUOpcodes::op_CB82, &CPUOpcodes::op_CB83,
		&CPUOpcodes::op_CB84, &CPUOpcodes::op_CB85,
		&CPUOpcodes::op_CB86, &CPUOpcodes::op_CB87,
		&CPUOpcodes::op_CB88, &CPUOpcodes::op_CB89,
		&CPUOpcodes::op_CB8A, &CPUOpcodes::op_CB8B,
		&CPUOpcodes::op_CB8C, &CPUOpcodes::op_CB8D,
		&CPUOpcodes::op_CB8E, &CPUOpcodes::op_CB8F,
		&CPUOpcodes::op_CB90, &CPUOpcodes::op_CB91,
		&CPUOpcodes::op_CB92, &CPUOpcodes::op_CB93,
		&CPUOpcodes::op_CB94, &CPUOpcodes::op_CB95,
		&CPUOpcodes::op_CB96, &CPUOpcodes::op_CB97,
		&CPUOpcodes::op_CB98, &CPUOpcodes::op_CB99,
		&CPUOpcodes::op_CB9A, &CPUOpcodes::op_CB9B,
		&CPUOpcodes::op_CB9C, &CPUOpcodes::op_CB9D,
		&CPUOpcodes::op_CB9E, &CPUOpcodes::op_CB9F,
		&CPUOpcodes::op_CBA0, &CPUOpcodes::op_CBA1,
		&CPUOpcodes::op_CBA2, &CPUOpcodes::op_CBA3,
		&CPUOpcodes::op_CBA4, &CPUOpcodes::op_CBA5,
		&CPUOpcodes::op_CBA6, &CPUOpcodes::op_CBA7,
		&CPUOpcodes::op_CBA8, &CPUOpcodes::op_CBA9,
		&CPUOpcodes::op_CBAA, &CPUOpcodes::op_CBAB,
		&CPUOpcodes::op_CBAC, &CPUOpcodes::op_CBAD,
		&CPUOpcodes::op_CBAE, &CPUOpcodes::op_CBAF,
		&CPUOpcodes::op_CBB0, &CPUOpcodes::op_CBB1,
		&CPUOpcodes::op_CBB2, &CPUOpcodes::op_CBB3,
		&CPUOpcodes::op_CBB4, &CPUOpcodes::op_CBB5,
		&CPUOpcodes::op_CBB6, &CPUOpcodes::op_CBB7,
		&CPUOpcodes::op_CBB8, &CPUOpcodes::op_CBB9,
		&CPUOpcodes::op_CBBA, &CPUOpcodes::op_CBBB,
		&CPUOpcodes::op_CBBC, &CPUOpcodes::op_CBBD,
		&CPUOpcodes::op_CBBE, &CPUOpcodes::op_CBBF,
		&CPUOpcodes::op_CBC0, &CPUOpcodes::op_CBC1,
		&CPUOpcodes::op_CBC2, &CPUOpcodes::op_CBC3,
		&CPUOpcodes::op_CBC4, &CPUOpcodes::op_CBC5,
		&CPUOpcodes::op_CBC6, &CPUOpcodes::op_CBC7,
		&CPUOpcodes::op_CBC8, &CPUOpcodes::op_CBC9,
		&CPUOpcodes::op_CBCA, &CPUOpcodes::op_CBCB,
		&CPUOpcodes::op_CBCC, &CPUOpcodes::op_CBCD,
		&CPUOpcodes::op_CBCE, &CPUOpcodes::op_CBCF,
		&CPUOpcodes::op_CBD0, &CPUOpcodes::op_CBD1,
		&CPUOpcodes::op_CBD2, &CPUOpcodes::op_CBD3,
		&CPUOpcodes::op_CBD4, &CPUOpcodes::op_CBD5,
		&CPUOpcodes::op_CBD6, &CPUOpcodes::op_CBD7,
		&CPUOpcodes::op_CBD8, &CPUOpcodes::op_CBD9,
		&CPUOpcodes::op_CBDA, &CPUOpcodes::op_CBDB,
		&CPUOpcodes::op_CBDC, &CPUOpcodes::op_CBDD,
		&CPUOpcodes::op_CBDE, &CPUOpcodes::op_CBDF,
		&CPUOpcodes::op_CBE0, &CPUOpcodes::op_CBE1,
		&CPUOpcodes::op_CBE2, &CPUOpcodes::op_CBE3,
		&CPUOpcodes::op_CBE4, &CPUOpcodes::op_CBE5,
		&CPUOpcodes::op_CBE6, &CPUOpcodes::op_CBE7,
		&CPUOpcodes::op_CBE8, &CPUOpcodes::op_CBE9,
		&CPUOpcodes::op_CBEA, &CPUOpcodes::op_CBEB,
		&CPUOpcodes::op_CBEC, &CPUOpcodes::op_CBED,
		&CPUOpcodes::op_CBEE, &CPUOpcodes::op_CBEF,
		&CPUOpcodes::op_CBF0, &CPUOpcodes::op_CBF1,
		&CPUOpcodes::op_CBF2, &CPUOpcodes::op_CBF3,
		&CPUOpcodes::op_CBF4, &CPUOpcodes::op_CBF5,
		&CPUOpcodes::op_CBF6, &CPUOpcodes::op_CBF7,
		&CPUOpcodes::op_CBF8, &CPUOpcodes::op_CBF9,
		&CPUOpcodes::op_CBFA, &CPUOpcodes::op_CBFB,
		&CPUOpcodes::op_CBFC, &CPUOpcodes::op_CBFD,
		&CPUOpcodes::op_CBFE, &CPUOpcodes::op_CBFF,
	};
};


