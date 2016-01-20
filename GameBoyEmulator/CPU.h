#pragma once

#include "CPURegisters.h"
#include "IMMU.h"
#include "Interrupts.h"
#include "Timer.h"

#define TEST_API __declspec(dllexport)

class TEST_API CPU
{
public:
	CPU(IMMU *mmu, CPURegisters *registers, Timer *timer, Interrupts *interrupts);
	~CPU();
	int ExecuteNextInstruction(Interrupts *interrupts);
	void NotifyInterruptOccurred();
	bool InStopMode();
private:
	CPURegisters *registers;
	IMMU *mmu;
	Timer *timer;
	Interrupts *interrupts;

	bool haltState;
	bool stopState;

	void TickTimer(int cycles);
	int ExecuteInstruction(int opcode, CPURegisters *registers);
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

	typedef int (CPU::*Instruction_Func)(CPURegisters *);
	Instruction_Func Instructions[0x100] =
	{
		&CPU::op_00, &CPU::op_01,
		&CPU::op_02, &CPU::op_03,
		&CPU::op_04, &CPU::op_05,
		&CPU::op_06, &CPU::op_07,
		&CPU::op_08, &CPU::op_09,
		&CPU::op_0A, &CPU::op_0B,
		&CPU::op_0C, &CPU::op_0D,
		&CPU::op_0E, &CPU::op_0F,
		&CPU::op_10, &CPU::op_11,
		&CPU::op_12, &CPU::op_13,
		&CPU::op_14, &CPU::op_15,
		&CPU::op_16, &CPU::op_17,
		&CPU::op_18, &CPU::op_19,
		&CPU::op_1A, &CPU::op_1B,
		&CPU::op_1C, &CPU::op_1D,
		&CPU::op_1E, &CPU::op_1F,
		&CPU::op_20, &CPU::op_21,
		&CPU::op_22, &CPU::op_23,
		&CPU::op_24, &CPU::op_25,
		&CPU::op_26, &CPU::op_27,
		&CPU::op_28, &CPU::op_29,
		&CPU::op_2A, &CPU::op_2B,
		&CPU::op_2C, &CPU::op_2D,
		&CPU::op_2E, &CPU::op_2F,
		&CPU::op_30, &CPU::op_31,
		&CPU::op_32, &CPU::op_33,
		&CPU::op_34, &CPU::op_35,
		&CPU::op_36, &CPU::op_37,
		&CPU::op_38, &CPU::op_39,
		&CPU::op_3A, &CPU::op_3B,
		&CPU::op_3C, &CPU::op_3D,
		&CPU::op_3E, &CPU::op_3F,
		&CPU::op_40, &CPU::op_41,
		&CPU::op_42, &CPU::op_43,
		&CPU::op_44, &CPU::op_45,
		&CPU::op_46, &CPU::op_47,
		&CPU::op_48, &CPU::op_49,
		&CPU::op_4A, &CPU::op_4B,
		&CPU::op_4C, &CPU::op_4D,
		&CPU::op_4E, &CPU::op_4F,
		&CPU::op_50, &CPU::op_51,
		&CPU::op_52, &CPU::op_53,
		&CPU::op_54, &CPU::op_55,
		&CPU::op_56, &CPU::op_57,
		&CPU::op_58, &CPU::op_59,
		&CPU::op_5A, &CPU::op_5B,
		&CPU::op_5C, &CPU::op_5D,
		&CPU::op_5E, &CPU::op_5F,
		&CPU::op_60, &CPU::op_61,
		&CPU::op_62, &CPU::op_63,
		&CPU::op_64, &CPU::op_65,
		&CPU::op_66, &CPU::op_67,
		&CPU::op_68, &CPU::op_69,
		&CPU::op_6A, &CPU::op_6B,
		&CPU::op_6C, &CPU::op_6D,
		&CPU::op_6E, &CPU::op_6F,
		&CPU::op_70, &CPU::op_71,
		&CPU::op_72, &CPU::op_73,
		&CPU::op_74, &CPU::op_75,
		&CPU::op_76, &CPU::op_77,
		&CPU::op_78, &CPU::op_79,
		&CPU::op_7A, &CPU::op_7B,
		&CPU::op_7C, &CPU::op_7D,
		&CPU::op_7E, &CPU::op_7F,
		&CPU::op_80, &CPU::op_81,
		&CPU::op_82, &CPU::op_83,
		&CPU::op_84, &CPU::op_85,
		&CPU::op_86, &CPU::op_87,
		&CPU::op_88, &CPU::op_89,
		&CPU::op_8A, &CPU::op_8B,
		&CPU::op_8C, &CPU::op_8D,
		&CPU::op_8E, &CPU::op_8F,
		&CPU::op_90, &CPU::op_91,
		&CPU::op_92, &CPU::op_93,
		&CPU::op_94, &CPU::op_95,
		&CPU::op_96, &CPU::op_97,
		&CPU::op_98, &CPU::op_99,
		&CPU::op_9A, &CPU::op_9B,
		&CPU::op_9C, &CPU::op_9D,
		&CPU::op_9E, &CPU::op_9F,
		&CPU::op_A0, &CPU::op_A1,
		&CPU::op_A2, &CPU::op_A3,
		&CPU::op_A4, &CPU::op_A5,
		&CPU::op_A6, &CPU::op_A7,
		&CPU::op_A8, &CPU::op_A9,
		&CPU::op_AA, &CPU::op_AB,
		&CPU::op_AC, &CPU::op_AD,
		&CPU::op_AE, &CPU::op_AF,
		&CPU::op_B0, &CPU::op_B1,
		&CPU::op_B2, &CPU::op_B3,
		&CPU::op_B4, &CPU::op_B5,
		&CPU::op_B6, &CPU::op_B7,
		&CPU::op_B8, &CPU::op_B9,
		&CPU::op_BA, &CPU::op_BB,
		&CPU::op_BC, &CPU::op_BD,
		&CPU::op_BE, &CPU::op_BF,
		&CPU::op_C0, &CPU::op_C1,
		&CPU::op_C2, &CPU::op_C3,
		&CPU::op_C4, &CPU::op_C5,
		&CPU::op_C6, &CPU::op_C7,
		&CPU::op_C8, &CPU::op_C9,
		&CPU::op_CA, &CPU::op_CB,
		&CPU::op_CC, &CPU::op_CD,
		&CPU::op_CE, &CPU::op_CF,
		&CPU::op_D0, &CPU::op_D1,
		&CPU::op_D2, nullptr,
		&CPU::op_D4, &CPU::op_D5,
		&CPU::op_D6, &CPU::op_D7,
		&CPU::op_D8, &CPU::op_D9,
		&CPU::op_DA, nullptr,
		&CPU::op_DC, nullptr,
		&CPU::op_DE, &CPU::op_DF,
		&CPU::op_E0, &CPU::op_E1,
		&CPU::op_E2, nullptr,
		nullptr,            &CPU::op_E5,
		&CPU::op_E6, &CPU::op_E7,
		&CPU::op_E8, &CPU::op_E9,
		&CPU::op_EA, nullptr,
		nullptr,            nullptr,
		&CPU::op_EE, &CPU::op_EF,
		&CPU::op_F0, &CPU::op_F1,
		&CPU::op_F2, &CPU::op_F3,
		nullptr,            &CPU::op_F5,
		&CPU::op_F6, &CPU::op_F7,
		&CPU::op_F8, &CPU::op_F9,
		&CPU::op_FA, &CPU::op_FB,
		nullptr,            nullptr,
		&CPU::op_FE, &CPU::op_FF,
	};

	Instruction_Func CB_Instructions[0x100] =
	{
		&CPU::op_CB00, &CPU::op_CB01,
		&CPU::op_CB02, &CPU::op_CB03,
		&CPU::op_CB04, &CPU::op_CB05,
		&CPU::op_CB06, &CPU::op_CB07,
		&CPU::op_CB08, &CPU::op_CB09,
		&CPU::op_CB0A, &CPU::op_CB0B,
		&CPU::op_CB0C, &CPU::op_CB0D,
		&CPU::op_CB0E, &CPU::op_CB0F,
		&CPU::op_CB10, &CPU::op_CB11,
		&CPU::op_CB12, &CPU::op_CB13,
		&CPU::op_CB14, &CPU::op_CB15,
		&CPU::op_CB16, &CPU::op_CB17,
		&CPU::op_CB18, &CPU::op_CB19,
		&CPU::op_CB1A, &CPU::op_CB1B,
		&CPU::op_CB1C, &CPU::op_CB1D,
		&CPU::op_CB1E, &CPU::op_CB1F,
		&CPU::op_CB20, &CPU::op_CB21,
		&CPU::op_CB22, &CPU::op_CB23,
		&CPU::op_CB24, &CPU::op_CB25,
		&CPU::op_CB26, &CPU::op_CB27,
		&CPU::op_CB28, &CPU::op_CB29,
		&CPU::op_CB2A, &CPU::op_CB2B,
		&CPU::op_CB2C, &CPU::op_CB2D,
		&CPU::op_CB2E, &CPU::op_CB2F,
		&CPU::op_CB30, &CPU::op_CB31,
		&CPU::op_CB32, &CPU::op_CB33,
		&CPU::op_CB34, &CPU::op_CB35,
		&CPU::op_CB36, &CPU::op_CB37,
		&CPU::op_CB38, &CPU::op_CB39,
		&CPU::op_CB3A, &CPU::op_CB3B,
		&CPU::op_CB3C, &CPU::op_CB3D,
		&CPU::op_CB3E, &CPU::op_CB3F,
		&CPU::op_CB40, &CPU::op_CB41,
		&CPU::op_CB42, &CPU::op_CB43,
		&CPU::op_CB44, &CPU::op_CB45,
		&CPU::op_CB46, &CPU::op_CB47,
		&CPU::op_CB48, &CPU::op_CB49,
		&CPU::op_CB4A, &CPU::op_CB4B,
		&CPU::op_CB4C, &CPU::op_CB4D,
		&CPU::op_CB4E, &CPU::op_CB4F,
		&CPU::op_CB50, &CPU::op_CB51,
		&CPU::op_CB52, &CPU::op_CB53,
		&CPU::op_CB54, &CPU::op_CB55,
		&CPU::op_CB56, &CPU::op_CB57,
		&CPU::op_CB58, &CPU::op_CB59,
		&CPU::op_CB5A, &CPU::op_CB5B,
		&CPU::op_CB5C, &CPU::op_CB5D,
		&CPU::op_CB5E, &CPU::op_CB5F,
		&CPU::op_CB60, &CPU::op_CB61,
		&CPU::op_CB62, &CPU::op_CB63,
		&CPU::op_CB64, &CPU::op_CB65,
		&CPU::op_CB66, &CPU::op_CB67,
		&CPU::op_CB68, &CPU::op_CB69,
		&CPU::op_CB6A, &CPU::op_CB6B,
		&CPU::op_CB6C, &CPU::op_CB6D,
		&CPU::op_CB6E, &CPU::op_CB6F,
		&CPU::op_CB70, &CPU::op_CB71,
		&CPU::op_CB72, &CPU::op_CB73,
		&CPU::op_CB74, &CPU::op_CB75,
		&CPU::op_CB76, &CPU::op_CB77,
		&CPU::op_CB78, &CPU::op_CB79,
		&CPU::op_CB7A, &CPU::op_CB7B,
		&CPU::op_CB7C, &CPU::op_CB7D,
		&CPU::op_CB7E, &CPU::op_CB7F,
		&CPU::op_CB80, &CPU::op_CB81,
		&CPU::op_CB82, &CPU::op_CB83,
		&CPU::op_CB84, &CPU::op_CB85,
		&CPU::op_CB86, &CPU::op_CB87,
		&CPU::op_CB88, &CPU::op_CB89,
		&CPU::op_CB8A, &CPU::op_CB8B,
		&CPU::op_CB8C, &CPU::op_CB8D,
		&CPU::op_CB8E, &CPU::op_CB8F,
		&CPU::op_CB90, &CPU::op_CB91,
		&CPU::op_CB92, &CPU::op_CB93,
		&CPU::op_CB94, &CPU::op_CB95,
		&CPU::op_CB96, &CPU::op_CB97,
		&CPU::op_CB98, &CPU::op_CB99,
		&CPU::op_CB9A, &CPU::op_CB9B,
		&CPU::op_CB9C, &CPU::op_CB9D,
		&CPU::op_CB9E, &CPU::op_CB9F,
		&CPU::op_CBA0, &CPU::op_CBA1,
		&CPU::op_CBA2, &CPU::op_CBA3,
		&CPU::op_CBA4, &CPU::op_CBA5,
		&CPU::op_CBA6, &CPU::op_CBA7,
		&CPU::op_CBA8, &CPU::op_CBA9,
		&CPU::op_CBAA, &CPU::op_CBAB,
		&CPU::op_CBAC, &CPU::op_CBAD,
		&CPU::op_CBAE, &CPU::op_CBAF,
		&CPU::op_CBB0, &CPU::op_CBB1,
		&CPU::op_CBB2, &CPU::op_CBB3,
		&CPU::op_CBB4, &CPU::op_CBB5,
		&CPU::op_CBB6, &CPU::op_CBB7,
		&CPU::op_CBB8, &CPU::op_CBB9,
		&CPU::op_CBBA, &CPU::op_CBBB,
		&CPU::op_CBBC, &CPU::op_CBBD,
		&CPU::op_CBBE, &CPU::op_CBBF,
		&CPU::op_CBC0, &CPU::op_CBC1,
		&CPU::op_CBC2, &CPU::op_CBC3,
		&CPU::op_CBC4, &CPU::op_CBC5,
		&CPU::op_CBC6, &CPU::op_CBC7,
		&CPU::op_CBC8, &CPU::op_CBC9,
		&CPU::op_CBCA, &CPU::op_CBCB,
		&CPU::op_CBCC, &CPU::op_CBCD,
		&CPU::op_CBCE, &CPU::op_CBCF,
		&CPU::op_CBD0, &CPU::op_CBD1,
		&CPU::op_CBD2, &CPU::op_CBD3,
		&CPU::op_CBD4, &CPU::op_CBD5,
		&CPU::op_CBD6, &CPU::op_CBD7,
		&CPU::op_CBD8, &CPU::op_CBD9,
		&CPU::op_CBDA, &CPU::op_CBDB,
		&CPU::op_CBDC, &CPU::op_CBDD,
		&CPU::op_CBDE, &CPU::op_CBDF,
		&CPU::op_CBE0, &CPU::op_CBE1,
		&CPU::op_CBE2, &CPU::op_CBE3,
		&CPU::op_CBE4, &CPU::op_CBE5,
		&CPU::op_CBE6, &CPU::op_CBE7,
		&CPU::op_CBE8, &CPU::op_CBE9,
		&CPU::op_CBEA, &CPU::op_CBEB,
		&CPU::op_CBEC, &CPU::op_CBED,
		&CPU::op_CBEE, &CPU::op_CBEF,
		&CPU::op_CBF0, &CPU::op_CBF1,
		&CPU::op_CBF2, &CPU::op_CBF3,
		&CPU::op_CBF4, &CPU::op_CBF5,
		&CPU::op_CBF6, &CPU::op_CBF7,
		&CPU::op_CBF8, &CPU::op_CBF9,
		&CPU::op_CBFA, &CPU::op_CBFB,
		&CPU::op_CBFC, &CPU::op_CBFD,
		&CPU::op_CBFE, &CPU::op_CBFF,
	};
};

