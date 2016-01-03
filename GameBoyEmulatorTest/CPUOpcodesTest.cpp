#include "stdafx.h"

#include "../GameBoyEmulator/CPUOpcodes.h"
#pragma comment(lib, "../Debug/GameBoyEmulator.lib")

#include "MockMMU.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace Microsoft::VisualStudio::TestTools::UnitTesting;

namespace GameBoyEmulatorTest
{
	[TestClass]
	public ref class CPUOpcodesTest
	{
	private:
		TestContext^ testContextInstance;

	public:
		/// <summary>
		///Gets or sets the test context which provides
		///information about and functionality for the current test run.
		///</summary>
		property Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ TestContext
		{
			Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ get()
			{
				return testContextInstance;
			}
			System::Void set(Microsoft::VisualStudio::TestTools::UnitTesting::TestContext^ value)
			{
				testContextInstance = value;
			}
		};

#pragma region Additional test attributes
		//
		//You can use the following additional attributes as you write your tests:
		//
		//Use ClassInitialize to run code before running the first test in the class
		//[ClassInitialize()]
		//static void MyClassInitialize(TestContext^ testContext) {};
		//
		//Use ClassCleanup to run code after all tests in a class have run
		//[ClassCleanup()]
		//static void MyClassCleanup() {};
		//
		//Use TestInitialize to run code before running each test
		//[TestInitialize()]
		//void MyTestInitialize() {};
		//
		//Use TestCleanup to run code after each test has run
		//[TestCleanup()]
		//void MyTestCleanup() {};
		//
#pragma endregion 

		[TestMethod]
		void Opcode_00()
		{
			MockMMU mmu;
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b00000000;
			int cycles = ops.op_00(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_01()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b00000000;
			int cycles = ops.op_01(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.b, 0xa7);
			Assert::AreEqual(registers.c, 0x34);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_02()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b00000000;
			int cycles = ops.op_02(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b00000000);
			// TODO: use moq to test if WriteByte() was called with appropriate parameters
		};

		[TestMethod]
		void Opcode_03_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x10;
			registers.c = 0xff;
			registers.f = 0b00000000;
			int cycles = ops.op_03(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x11);
			Assert::AreEqual(registers.c, 0x00);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_03_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0xff;
			registers.c = 0xff;
			registers.f = 0b00000000;
			int cycles = ops.op_03(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x00);
			Assert::AreEqual(registers.c, 0x00);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_04_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x45;
			registers.f = 0b00010000;
			int cycles = ops.op_04(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x46);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_04_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0xff;
			registers.f = 0b00010000;
			int cycles = ops.op_04(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_04_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x0f;
			registers.f = 0b00010000;
			int cycles = ops.op_04(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_05_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x45;
			registers.f = 0b00010000;
			int cycles = ops.op_05(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_05_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x00;
			registers.f = 0b00010000;
			int cycles = ops.op_05(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_05_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x01;
			registers.f = 0b00010000;
			int cycles = ops.op_05(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_05_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x10;
			registers.f = 0b00010000;
			int cycles = ops.op_05(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_06()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x10;
			registers.f = 0b00000000;
			int cycles = ops.op_06(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.b, 0x4c);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_07_Carry_NoChange()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xff;
			registers.f = 0b11100000;
			int cycles = ops.op_07(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_07_Carry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xc3;
			registers.f = 0b11100000;
			int cycles = ops.op_07(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x87);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_07_NoCarry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x7f;
			registers.f = 0b11100000;
			int cycles = ops.op_07(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xfe);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_08()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xffa0;
			registers.f = 0b11110000;
			int cycles = ops.op_08(&registers);
			Assert::AreEqual(cycles, 20);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 0xffa0);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_09_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x23;
			registers.l = 0x56;
			registers.b = 0x10;
			registers.c = 0x07;
			registers.f = 0b11110000;
			int cycles = ops.op_09(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x33);
			Assert::AreEqual(registers.l, 0x5d);
			Assert::AreEqual(registers.b, 0x10);
			Assert::AreEqual(registers.c, 0x07);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_09_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0xff;
			registers.l = 0xf0;
			registers.b = 0x00;
			registers.c = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_09(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.b, 0x00);
			Assert::AreEqual(registers.c, 0x10);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_09_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x0e;
			registers.l = 0x00;
			registers.b = 0x02;
			registers.c = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_09(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.b, 0x02);
			Assert::AreEqual(registers.c, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_0A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0;
			registers.f = 0b11110000;
			int cycles = ops.op_0A(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_0B_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0;
			registers.c = 0;
			registers.f = 0b11110000;
			int cycles = ops.op_0B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0xff);
			Assert::AreEqual(registers.c, 0xff);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_0B_NoUnderflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0xff;
			registers.c = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_0B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0xff);
			Assert::AreEqual(registers.c, 0xfe);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_0C_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x0f;
			registers.f = 0b11110000;
			int cycles = ops.op_0C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_0C_NoZero_NoHalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_0C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x11);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_0C_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_0C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_0D_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x45;
			registers.f = 0b11110000;
			int cycles = ops.op_0D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_0D_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_0D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_0D_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x01;
			registers.f = 0b11110000;
			int cycles = ops.op_0D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_0D_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x10;
			registers.f = 0b00010000;
			int cycles = ops.op_0D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_0E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_0E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.c, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_0F_Carry_NoChange()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xff;
			registers.f = 0b11100000;
			int cycles = ops.op_0F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_0F_Carry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xc3;
			registers.f = 0b11100000;
			int cycles = ops.op_0F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xE1);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_0F_NoCarry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xFE;
			registers.f = 0b11110000;
			int cycles = ops.op_0F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x7f);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_11()
		{
			MockMMU mmu(0x4c, 0xb130);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_11(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.d, 0xb1);
			Assert::AreEqual(registers.e, 0x30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_12()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_12(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to test if WriteByte() was called with appropriate parameters
		};

		[TestMethod]
		void Opcode_13_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x10;
			registers.e = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_13(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x11);
			Assert::AreEqual(registers.e, 0x00);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_13_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0xff;
			registers.e = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_13(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x00);
			Assert::AreEqual(registers.e, 0x00);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_14_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x45;
			registers.f = 0b01010000;
			int cycles = ops.op_14(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x46);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_14_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0xff;
			registers.f = 0b01010000;
			int cycles = ops.op_14(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_14_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x0f;
			registers.f = 0b01010000;
			int cycles = ops.op_14(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_15_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x45;
			registers.f = 0b11110000;
			int cycles = ops.op_15(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_15_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x00;
			registers.f = 0b10010000;
			int cycles = ops.op_15(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_15_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x01;
			registers.f = 0b00110000;
			int cycles = ops.op_15(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_15_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_15(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_16()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_16(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.d, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_17_Carry_NoChange()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_17(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_17_Carry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xc3;
			registers.f = 0b11100000;
			int cycles = ops.op_17(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x86);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_17_NoCarry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x7f;
			registers.f = 0b11100000;
			int cycles = ops.op_17(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xfe);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_17_Carry_ExistingCarry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x7f;
			registers.f = 0b11110000;
			int cycles = ops.op_17(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_18_JumpForward()
		{
			MockMMU mmu(0x5, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_18(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 16);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_18_JumpBackward()
		{
			MockMMU mmu(0xfe, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_18(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 9);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_19_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x23;
			registers.l = 0x56;
			registers.d = 0x20;
			registers.e = 0x07;
			registers.f = 0b11110000;
			int cycles = ops.op_19(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x43);
			Assert::AreEqual(registers.l, 0x5d);
			Assert::AreEqual(registers.d, 0x20);
			Assert::AreEqual(registers.e, 0x07);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_19_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0xff;
			registers.l = 0xf0;
			registers.d = 0x00;
			registers.e = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_19(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.d, 0x00);
			Assert::AreEqual(registers.e, 0x10);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_19_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x0e;
			registers.l = 0x00;
			registers.d = 0x02;
			registers.e = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_19(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.d, 0x02);
			Assert::AreEqual(registers.e, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_1A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0;
			registers.f = 0b11110000;
			int cycles = ops.op_1A(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_1B_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0;
			registers.e = 0;
			registers.f = 0b11110000;
			int cycles = ops.op_1B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0xff);
			Assert::AreEqual(registers.e, 0xff);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_1B_NoUnderflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0xff;
			registers.e = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_1B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0xff);
			Assert::AreEqual(registers.e, 0xfe);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_1C_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x0f;
			registers.f = 0b11110000;
			int cycles = ops.op_1C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_1C_NoZero_NoHalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_1C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x11);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_1C_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_1C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_1D_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x45;
			registers.f = 0b11110000;
			int cycles = ops.op_1D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_1D_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_1D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_1D_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x01;
			registers.f = 0b11110000;
			int cycles = ops.op_1D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_1D_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_1D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_1E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_1E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.e, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_1F_Carry_NoChange()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_1F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_1F_Carry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xc3;
			registers.f = 0b11100000;
			int cycles = ops.op_1F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x61);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_1F_NoCarry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11100000;
			int cycles = ops.op_1F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x7f);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_1F_Carry_ExistingCarry_Change()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11110000;
			int cycles = ops.op_1F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_20_ZeroFlag()
		{
			MockMMU mmu(0x5, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_20(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_20_NoZeroFlag_JumpBackward()
		{
			MockMMU mmu(0xfe, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b01110000;
			int cycles = ops.op_20(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 9);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_20_NoZeroFlag_JumpForward()
		{
			MockMMU mmu(0x5, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b01110000;
			int cycles = ops.op_20(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 16);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_21()
		{
			MockMMU mmu(0x4c, 0xb130);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_21(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.h, 0xb1);
			Assert::AreEqual(registers.l, 0x30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_22()
		{
			MockMMU mmu(0x4c, 0xb130);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x4e;
			registers.l = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_22(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x4f);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure WriteByte() was called with correct address.
		};

		[TestMethod]
		void Opcode_23_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x10;
			registers.l = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_23(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x11);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_23_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0xff;
			registers.l = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_23(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_24_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x45;
			registers.f = 0b11110000;
			int cycles = ops.op_24(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x46);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_24_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0xff;
			registers.f = 0b01010000;
			int cycles = ops.op_24(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_24_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x0f;
			registers.f = 0b11010000;
			int cycles = ops.op_24(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_25_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x45;
			registers.f = 0b10110000;
			int cycles = ops.op_25(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_25_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x00;
			registers.f = 0b10010000;
			int cycles = ops.op_25(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_25_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x01;
			registers.f = 0b00110000;
			int cycles = ops.op_25(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_25_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_25(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_26()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_26(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_27_Add_NoCorrection()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b10000000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_27_Add_6Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x1A;
			registers.f = 0b10000000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x20);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_27_Add_HalfCarry_6Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.f = 0b10100000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x18);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_27_Add_60Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xA8;
			registers.f = 0b10000000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x08);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_27_Add_66Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x9A;
			registers.f = 0b10000000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_27_Add_HalfCarry_66Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xA0;
			registers.f = 0b10100000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x06);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_27_Add_Carry_60Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x29;
			registers.f = 0b10010000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x89);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_27_Add_Carry_66Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x2A;
			registers.f = 0b10010000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x90);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_27_Add_Carry_HalfCarry_66Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x30;
			registers.f = 0b10110000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x96);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_27_Subtract_NoCorrection()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x99;
			registers.f = 0b11000000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x99);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_27_Subtract_HalfCarry_Sub6Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x86;
			registers.f = 0b11100000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_27_Subtract_Carry_Sub60Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x70;
			registers.f = 0b11010000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_27_Subtract_Carry_HalfCarry_Sub66Correction()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x66;
			registers.f = 0b11110000;
			int cycles = ops.op_27(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_28_ZeroFlag_JumpForward()
		{
			MockMMU mmu(0x5, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_28(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 16);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_28_NoZeroFlag()
		{
			MockMMU mmu(0xfe, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b01110000;
			int cycles = ops.op_28(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_29_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x23;
			registers.l = 0x56;
			registers.f = 0b11110000;
			int cycles = ops.op_29(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x46);
			Assert::AreEqual(registers.l, 0xac);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_29_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x88;
			registers.l = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_29(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_29_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x08;
			registers.l = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_29(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_2A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x4;
			registers.h = 0x40;
			registers.l = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_2A(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.h, 0x40);
			Assert::AreEqual(registers.l, 0x01);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_2A_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x4;
			registers.h = 0xff;
			registers.l = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_2A(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_2B_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0;
			registers.l = 0;
			registers.f = 0b11110000;
			int cycles = ops.op_2B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0xff);
			Assert::AreEqual(registers.l, 0xff);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_2B_NoUnderflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0xff;
			registers.l = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_2B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0xff);
			Assert::AreEqual(registers.l, 0xfe);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_2C_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x0f;
			registers.f = 0b11010000;
			int cycles = ops.op_2C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_2C_NoZero_NoHalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_2C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x11);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_2C_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0xff;
			registers.f = 0b01010000;
			int cycles = ops.op_2C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_2D_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x45;
			registers.f = 0b10110000;
			int cycles = ops.op_2D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_2D_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x00;
			registers.f = 0b10010000;
			int cycles = ops.op_2D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_2D_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x01;
			registers.f = 0b00110000;
			int cycles = ops.op_2D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_2D_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_2D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_2E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_2E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.l, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_2F()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b10100101;
			registers.f = 0b10010000;
			int cycles = ops.op_2F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b01011010);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_30_CarryFlag()
		{
			MockMMU mmu(0x5, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_30(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_30_NoCarryFlag_JumpBackward()
		{
			MockMMU mmu(0xfe, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11100000;
			int cycles = ops.op_30(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 9);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_31()
		{
			MockMMU mmu(0x4c, 0xb130);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_31(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 0xb130);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_32()
		{
			MockMMU mmu(0x4c, 0xb130);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x4e;
			registers.l = 0xff;
			registers.f = 0b11110000;
			int cycles = ops.op_32(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x4e);
			Assert::AreEqual(registers.l, 0xfe);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure WriteByte() was called with correct address.
		};

		[TestMethod]
		void Opcode_32_Underflow()
		{
			MockMMU mmu(0x4c, 0xb130);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x00;
			registers.l = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_32(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0xff);
			Assert::AreEqual(registers.l, 0xff);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure WriteByte() was called with correct address.
		};

		[TestMethod]
		void Opcode_33_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0x10ff;
			registers.f = 0b11110000;
			int cycles = ops.op_33(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 0x1100);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_33_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xffff;
			registers.f = 0b11110000;
			int cycles = ops.op_33(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 0x0000);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_34()
		{
			// TODO: Write tests for op_34() by making sure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_35()
		{
			// TODO: Write tests for op_35() by making sure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_36()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_36(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: Write tests for op_35() by making sure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_37()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11100000;
			int cycles = ops.op_37(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_38_CarryFlag_JumpForward()
		{
			MockMMU mmu(0x5, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_38(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 16);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_38_NoCarryFlag()
		{
			MockMMU mmu(0xfe, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11100000;
			int cycles = ops.op_38(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_39_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x23;
			registers.l = 0x56;
			registers.sp = 0x2007;
			registers.f = 0b10000000;
			int cycles = ops.op_39(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x43);
			Assert::AreEqual(registers.l, 0x5d);
			Assert::AreEqual(registers.sp, 0x2007);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_39_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0xff;
			registers.l = 0xf0;
			registers.sp = 0x0010;
			registers.f = 0b11000000;
			int cycles = ops.op_39(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.sp, 0x0010);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_39_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x0e;
			registers.l = 0x00;
			registers.sp = 0x0200;
			registers.f = 0b11010000;
			int cycles = ops.op_39(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.sp, 0x0200);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_3A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x4;
			registers.h = 0x40;
			registers.l = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_3A(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.h, 0x3f);
			Assert::AreEqual(registers.l, 0xff);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_3A_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x4;
			registers.h = 0x00;
			registers.l = 0x00;
			registers.f = 0b11110000;
			int cycles = ops.op_3A(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.h, 0xff);
			Assert::AreEqual(registers.l, 0xff);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_3B_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0x0000;
			registers.f = 0b11110000;
			int cycles = ops.op_3B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 0xffff);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_3B_NoUnderflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xffff;
			registers.f = 0b11110000;
			int cycles = ops.op_3B(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 0xfffe);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_3C_HalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x0f;
			registers.f = 0b11010000;
			int cycles = ops.op_3C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_3C_NoZero_NoHalfOverflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_3C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x11);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_3C_Overflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xff;
			registers.f = 0b01010000;
			int cycles = ops.op_3C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_3D_NoZeroFlag_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x45;
			registers.f = 0b10110000;
			int cycles = ops.op_3D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x44);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_3D_Underflow()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.f = 0b10010000;
			int cycles = ops.op_3D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xff);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_3D_ZeroFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x01;
			registers.f = 0b00110000;
			int cycles = ops.op_3D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11010000);
		};

		[TestMethod]
		void Opcode_3D_NoZeroFlag_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_3D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_3E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_3E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_3F()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_3F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_3F_NoCarryFlag()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11100000;
			int cycles = ops.op_3F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_40()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_40(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_41()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.c = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_41(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_42()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.d = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_42(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_43()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.e = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_43(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_44()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.h = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_44(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_45()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_45(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_46()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_46(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_47()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.a = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_47(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_48()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.b = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_48(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_49()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_49(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_4A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.d = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_4A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_4B()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.e = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_4B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_4C()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.h = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_4C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_4D()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_4D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_4E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_4E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_4F()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.a = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_4F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_50()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.b = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_50(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_51()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.c = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_51(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_52()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_52(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_53()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.e = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_53(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_54()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.h = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_54(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_55()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_55(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_56()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_56(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_57()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.a = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_57(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_58()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.b = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_58(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_59()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.c = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_59(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_5A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.d = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_5A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_5B()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_5B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_5C()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.h = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_5C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_5D()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_5D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_5E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_5E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_5F()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.a = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_5F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_60()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.b = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_60(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_61()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.c = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_61(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_62()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.d = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_62(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_63()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.e = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_63(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_64()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_64(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_65()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_65(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_66()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_66(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_67()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.a = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_67(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_68()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.b = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_68(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_69()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.c = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_69(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_6A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.d = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_6A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_6B()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.e = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_6B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_6C()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.h = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_6C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_6D()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_6D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_6E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_6E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_6F()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.a = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_6F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_70()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_70(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_71()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_71(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_72()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_72(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_73()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_73(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_74()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_74(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_75()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.l = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_75(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.l, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_76()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_76(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: figure out how to test halt
		};

		[TestMethod]
		void Opcode_77()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_77(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to ensure WriteByte() is called correctly.
		};

		[TestMethod]
		void Opcode_78()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.b = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_78(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.b, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_79()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.c = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_79(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.c, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_7A()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.d = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_7A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.d, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_7B()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.e = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_7B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.e, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_7C()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.h = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_7C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.h, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_7D()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_7D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x50);
			Assert::AreEqual(registers.l, 0x50);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_7E()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_7E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: use moq to make sure ReadByte() was called correctly
		};

		[TestMethod]
		void Opcode_7F()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x40;
			registers.f = 0b11110000;
			int cycles = ops.op_7F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x40);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_80_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_80(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_80_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.b = 0x1c;
			registers.f = 0b11110000;
			int cycles = ops.op_80(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_80_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.b = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_80(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_80_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.b = 0x12;
			registers.f = 0b11000000;
			int cycles = ops.op_80(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_80_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_80(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_81_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.c = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_81(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_81_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.c = 0x1c;
			registers.f = 0b11110000;
			int cycles = ops.op_81(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_81_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.c = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_81(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_81_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.c = 0x12;
			registers.f = 0b11000000;
			int cycles = ops.op_81(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_81_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.c = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_81(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_82_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.d = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_82(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_82_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.d = 0x1c;
			registers.f = 0b11110000;
			int cycles = ops.op_82(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_82_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.d = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_82(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_82_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.d = 0x12;
			registers.f = 0b11000000;
			int cycles = ops.op_82(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_82_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.d = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_82(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_83_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.e = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_83(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_83_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.e = 0x1c;
			registers.f = 0b11110000;
			int cycles = ops.op_83(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_83_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.e = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_83(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_83_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.e = 0x12;
			registers.f = 0b11000000;
			int cycles = ops.op_83(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_83_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.e = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_83(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_84_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.h = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_84(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_84_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.h = 0x1c;
			registers.f = 0b11110000;
			int cycles = ops.op_84(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_84_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.h = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_84(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_84_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.h = 0x12;
			registers.f = 0b11000000;
			int cycles = ops.op_84(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_84_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.h = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_84(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_85_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.l = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_85(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_85_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.l = 0x1c;
			registers.f = 0b11110000;
			int cycles = ops.op_85(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_85_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.l = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_85(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_85_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.l = 0x12;
			registers.f = 0b11000000;
			int cycles = ops.op_85(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_85_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.l = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_85(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_86_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x13, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b11110000;
			int cycles = ops.op_86(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_86_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x1c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.f = 0b11110000;
			int cycles = ops.op_86(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_86_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11100000;
			int cycles = ops.op_86(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_86_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x12, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11000000;
			int cycles = ops.op_86(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_86_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b01000000;
			int cycles = ops.op_86(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_87_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b11110000;
			int cycles = ops.op_87(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x22);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_87_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x18;
			registers.f = 0b11110000;
			int cycles = ops.op_87(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_87_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xf1;
			registers.f = 0b11100000;
			int cycles = ops.op_87(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xe2);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_87_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x88;
			registers.f = 0b11000000;
			int cycles = ops.op_87(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_87_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x80;
			registers.f = 0b01000000;
			int cycles = ops.op_87(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_88_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_88(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_88_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.b = 0x1b;
			registers.f = 0b11110000;
			int cycles = ops.op_88(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_88_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.b = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_88(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_88_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.b = 0x11;
			registers.f = 0b11010000;
			int cycles = ops.op_88(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_88_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_88(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_89_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.c = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_89(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_89_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.c = 0x1b;
			registers.f = 0b11110000;
			int cycles = ops.op_89(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_89_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.c = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_89(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_89_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.c = 0x11;
			registers.f = 0b11010000;
			int cycles = ops.op_89(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_89_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.c = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_89(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_8A_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.d = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_8A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_8A_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.d = 0x1b;
			registers.f = 0b11110000;
			int cycles = ops.op_8A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_8A_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.d = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_8A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_8A_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.d = 0x11;
			registers.f = 0b11010000;
			int cycles = ops.op_8A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_8A_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.d = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_8A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_8B_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.e = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_8B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_8B_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.e = 0x1b;
			registers.f = 0b11110000;
			int cycles = ops.op_8B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_8B_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.e = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_8B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_8B_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.e = 0x11;
			registers.f = 0b11010000;
			int cycles = ops.op_8B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_8B_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.e = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_8B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_8C_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.h = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_8C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_8C_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.h = 0x1b;
			registers.f = 0b11110000;
			int cycles = ops.op_8C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_8C_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.h = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_8C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_8C_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.h = 0x11;
			registers.f = 0b11010000;
			int cycles = ops.op_8C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_8C_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.h = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_8C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_8D_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.l = 0x13;
			registers.f = 0b11110000;
			int cycles = ops.op_8D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_8D_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.l = 0x1b;
			registers.f = 0b11110000;
			int cycles = ops.op_8D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_8D_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.l = 0x10;
			registers.f = 0b11100000;
			int cycles = ops.op_8D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_8D_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.l = 0x11;
			registers.f = 0b11010000;
			int cycles = ops.op_8D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_8D_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.l = 0xef;
			registers.f = 0b01000000;
			int cycles = ops.op_8D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_8E_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x13, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b11110000;
			int cycles = ops.op_8E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_8E_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x1c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.f = 0b11100000;
			int cycles = ops.op_8E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_8E_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfd;
			registers.f = 0b11110000;
			int cycles = ops.op_8E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_8E_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x12, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfd;
			registers.f = 0b11010000;
			int cycles = ops.op_8E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_8E_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b01000000;
			int cycles = ops.op_8E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_8F_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b11110000;
			int cycles = ops.op_8F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x23);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_8F_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x18;
			registers.f = 0b11110000;
			int cycles = ops.op_8F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x31);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_8F_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xf1;
			registers.f = 0b11100000;
			int cycles = ops.op_8F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xe2);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_8F_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x88;
			registers.f = 0b11010000;
			int cycles = ops.op_8F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x11);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_8F_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x80;
			registers.f = 0b01000000;
			int cycles = ops.op_8F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_90_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_90(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_90_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.b = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_90(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_90_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.b = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_90(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_90_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.b = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_90(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_90_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_90(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_91_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.c = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_91(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_91_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.c = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_91(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_91_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.c = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_91(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_91_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.c = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_91(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_91_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.c = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_91(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_92_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.d = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_92(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_92_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.d = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_92(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_92_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.d = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_92(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_92_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.d = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_92(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_92_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.d = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_92(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_93_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.e = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_93(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_93_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.e = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_93(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_93_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.e = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_93(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_93_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.e = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_93(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_93_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.e = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_93(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_94_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.h = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_94(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_94_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.h = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_94(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_94_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.h = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_94(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_94_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.h = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_94(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_94_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.h = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_94(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_95_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.l = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_95(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_95_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.l = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_95(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_95_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.l = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_95(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_95_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.l = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_95(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_95_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.l = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_95(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_96_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x01, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b10110000;
			int cycles = ops.op_96(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_96_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x05, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.f = 0b10010000;
			int cycles = ops.op_96(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_96_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x80, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.f = 0b10100000;
			int cycles = ops.op_96(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_96_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x41, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.f = 0b11000000;
			int cycles = ops.op_96(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_96_Zero()
		{
			MockMMU mmu(0x11, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_96(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_97_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x88;
			registers.f = 0b00110000;
			int cycles = ops.op_97(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_98_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.b = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_98(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_98_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.b = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_98(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_98_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.b = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_98(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_98_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.b = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_98(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_98_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.b = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_98(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_99_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.c = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_99(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_99_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.c = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_99(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_99_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.c = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_99(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_99_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.c = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_99(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_99_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.c = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_99(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_9A_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.d = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_9A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_9A_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.d = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_9A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_9A_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.d = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_9A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_9A_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.d = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_9A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_9A_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.d = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_9A(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_9B_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.e = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_9B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_9B_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.e = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_9B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_9B_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.e = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_9B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_9B_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.e = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_9B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_9B_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.e = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_9B(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_9C_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.h = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_9C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_9C_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.h = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_9C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_9C_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.h = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_9C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_9C_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.h = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_9C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_9C_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.h = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_9C(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_9D_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.l = 0x01;
			registers.f = 0b10110000;
			int cycles = ops.op_9D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_9D_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.l = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_9D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_9D_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.l = 0x80;
			registers.f = 0b10100000;
			int cycles = ops.op_9D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_9D_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.l = 0x41;
			registers.f = 0b11000000;
			int cycles = ops.op_9D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_9D_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.l = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_9D(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_9E_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x01, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.f = 0b10110000;
			int cycles = ops.op_9E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_9E_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x05, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.f = 0b10010000;
			int cycles = ops.op_9E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_9E_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x80, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.f = 0b10100000;
			int cycles = ops.op_9E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_9E_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x41, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.f = 0b11000000;
			int cycles = ops.op_9E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_9E_Zero()
		{
			MockMMU mmu(0x11, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.f = 0b00110000;
			int cycles = ops.op_9E(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_9F_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x88;
			registers.f = 0b00110000;
			int cycles = ops.op_9F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0xFF);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_9F_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x88;
			registers.f = 0b00100000;
			int cycles = ops.op_9F(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_A0_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.b = 0b11110000;
			registers.f = 0b01010000;
			int cycles = ops.op_A0(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A0_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.b = 0b11110000;
			registers.f = 0b11010000;
			int cycles = ops.op_A0(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A1_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.c = 0b11110000;
			registers.f = 0b01010000;
			int cycles = ops.op_A1(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A1_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.c = 0b11110000;
			registers.f = 0b11010000;
			int cycles = ops.op_A1(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A2_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.d = 0b11110000;
			registers.f = 0b01010000;
			int cycles = ops.op_A2(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A2_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.d = 0b11110000;
			registers.f = 0b11010000;
			int cycles = ops.op_A2(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A3_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.e = 0b11110100;
			registers.f = 0b01010000;
			int cycles = ops.op_A3(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A3_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b10011100;
			registers.e = 0b11110110;
			registers.f = 0b11010000;
			int cycles = ops.op_A3(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10010100);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A4_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.h = 0b11110000;
			registers.f = 0b01010000;
			int cycles = ops.op_A4(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A4_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.h = 0b11110000;
			registers.f = 0b11010000;
			int cycles = ops.op_A4(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A5_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.l = 0b11110000;
			registers.f = 0b01010000;
			int cycles = ops.op_A5(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A5_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.l = 0b11110000;
			registers.f = 0b11010000;
			int cycles = ops.op_A5(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A6_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.f = 0b01010000;
			int cycles = ops.op_A6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A6_NonZero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11010000;
			int cycles = ops.op_A6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A7_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.b = 0b11110000;
			registers.f = 0b01010000;
			int cycles = ops.op_A7(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_A7_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11010000;
			int cycles = ops.op_A7(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00011100);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_A8_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.b = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_A8(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_A8_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.b = 0b11110000;
			registers.f = 0b11110000;
			int cycles = ops.op_A8(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_A9_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.c = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_A9(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_A9_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.c = 0b11110000;
			registers.f = 0b11110000;
			int cycles = ops.op_A9(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_AA_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.d = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_AA(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_AA_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.d = 0b11110000;
			registers.f = 0b11110000;
			int cycles = ops.op_AA(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_AB_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.e = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_AB(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_AB_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.e = 0b11110000;
			registers.f = 0b11110000;
			int cycles = ops.op_AB(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_AC_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.h = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_AC(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_AC_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.h = 0b11110000;
			registers.f = 0b11110000;
			int cycles = ops.op_AC(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_AD_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.l = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_AD(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_AD_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.l = 0b11110000;
			registers.f = 0b11110000;
			int cycles = ops.op_AD(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_AE_Zero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_AE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_AE_NonZero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11110000;
			int cycles = ops.op_AE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_AF_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_AF(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B0_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.b = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B0(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B0_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.b = 0b10110000;
			registers.f = 0b11110000;
			int cycles = ops.op_B0(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B1_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.c = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B1(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B1_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.c = 0b10110000;
			registers.f = 0b11110000;
			int cycles = ops.op_B1(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B2_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.d = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B2(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B2_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.d = 0b10110000;
			registers.f = 0b11110000;
			int cycles = ops.op_B2(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B3_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.e = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B3(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B3_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.e = 0b10110000;
			registers.f = 0b11110000;
			int cycles = ops.op_B3(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B4_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.h = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B4(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B4_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.h = 0b10110000;
			registers.f = 0b11110000;
			int cycles = ops.op_B4(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B5_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.l = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B5(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B5_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.l = 0b10110000;
			registers.f = 0b11110000;
			int cycles = ops.op_B5(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B6_Zero()
		{
			MockMMU mmu(0b00000000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B6_NonZero()
		{
			MockMMU mmu(0b10110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11110000;
			int cycles = ops.op_B6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B7_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_B7(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_B7_NonZero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11110000;
			int cycles = ops.op_B7(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00011100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_B8_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.b = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_B8(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.b, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_B8_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.b = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_B8(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.b, 0x05);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_B8_Carry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.b = 0x20;
			registers.f = 0b10100000;
			int cycles = ops.op_B8(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.b, 0x20);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_B9_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.c = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_B9(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.c, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_B9_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.c = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_B9(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.c, 0x05);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_B9_Carry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.c = 0x20;
			registers.f = 0b10100000;
			int cycles = ops.op_B9(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.c, 0x20);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_BA_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.d = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_BA(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.d, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_BA_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.d = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_BA(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.d, 0x05);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_BA_Carry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.d = 0x20;
			registers.f = 0b10100000;
			int cycles = ops.op_BA(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.d, 0x20);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_BB_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.e = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_BB(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.e, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_BB_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.e = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_BB(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.e, 0x05);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_BB_Carry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.e = 0x20;
			registers.f = 0b10100000;
			int cycles = ops.op_BB(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.e, 0x20);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_BC_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.h = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_BC(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.h, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_BC_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.h = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_BC(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.h, 0x05);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_BC_Carry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.h = 0x20;
			registers.f = 0b10100000;
			int cycles = ops.op_BC(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.h, 0x20);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_BD_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.l = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_BD(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.l, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_BD_HalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.l = 0x05;
			registers.f = 0b10010000;
			int cycles = ops.op_BD(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.l, 0x05);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_BD_Carry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.l = 0x20;
			registers.f = 0b10100000;
			int cycles = ops.op_BD(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.l, 0x20);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_BE_Zero()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_BE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_BE_HalfCarry()
		{
			MockMMU mmu(0x05, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_BE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_BE_Carry()
		{
			MockMMU mmu(0x20, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b10100000;
			int cycles = ops.op_BE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_BF_Zero()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_BF(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_C0_Zero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C0(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C0_NotZero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b01110000;
			int cycles = ops.op_C0(&registers);
			Assert::AreEqual(cycles, 20);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_C1()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.b = 1;
			registers.c = 2;
			registers.f = 0b11110000;
			int cycles = ops.op_C1(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.b, 0x4c);
			Assert::AreEqual(registers.c, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C2_Zero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C2(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C2_NotZero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b01110000;
			int cycles = ops.op_C2(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_C3()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C3(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C4_Zero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C4(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C4_NotZero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b01110000;
			int cycles = ops.op_C4(&registers);
			Assert::AreEqual(cycles, 24);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_C5()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.b = 0x05;
			registers.c = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_C5(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.b, 0x05);
			Assert::AreEqual(registers.c, 0x10);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C6_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x13, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b11110000;
			int cycles = ops.op_C6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x24);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_C6_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x1c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.f = 0b11110000;
			int cycles = ops.op_C6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_C6_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11100000;
			int cycles = ops.op_C6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_C6_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x12, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11000000;
			int cycles = ops.op_C6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_C6_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b01000000;
			int cycles = ops.op_C6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_C7()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C7(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C8_Zero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C8(&registers);
			Assert::AreEqual(cycles, 20);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_C8_NotZero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b01110000;
			int cycles = ops.op_C8(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_C9()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_C9(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CA_Zero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_CA(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CA_NotZero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b01110000;
			int cycles = ops.op_CA(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_CC_Zero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_CC(&registers);
			Assert::AreEqual(cycles, 24);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CC_NotZero()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b01110000;
			int cycles = ops.op_CC(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_CD()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_CD(&registers);
			Assert::AreEqual(cycles, 24);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CE_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x13, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b11110000;
			int cycles = ops.op_CE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x25);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CE_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x1b, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.f = 0b11110000;
			int cycles = ops.op_CE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x30);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_CE_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11100000;
			int cycles = ops.op_CE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x0e);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CE_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x11, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0xfe;
			registers.f = 0b11010000;
			int cycles = ops.op_CE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_CE_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b01000000;
			int cycles = ops.op_CE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_CF()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_CF(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 8);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D0_Carry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_D0(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D0_NotCarry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11100000;
			int cycles = ops.op_D0(&registers);
			Assert::AreEqual(cycles, 20);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_D1()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.d = 1;
			registers.e = 2;
			registers.f = 0b11110000;
			int cycles = ops.op_D1(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.d, 0x4c);
			Assert::AreEqual(registers.e, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D2_Carry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_D2(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D2_NotCarry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11100000;
			int cycles = ops.op_D2(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_D4_Carry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_D4(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D4_NotCarry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11100000;
			int cycles = ops.op_D4(&registers);
			Assert::AreEqual(cycles, 24);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_D5()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.d = 0x05;
			registers.e = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_D5(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.d, 0x05);
			Assert::AreEqual(registers.e, 0x10);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D6_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x01, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b10110000;
			int cycles = ops.op_D6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_D6_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x05, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x14;
			registers.f = 0b10010000;
			int cycles = ops.op_D6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_D6_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x80, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.f = 0b10100000;
			int cycles = ops.op_D6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_D6_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x41, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.f = 0b11000000;
			int cycles = ops.op_D6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_D6_Zero()
		{
			MockMMU mmu(0x11, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_D6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_D7()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_D7(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x10);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D8_Carry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_D8(&registers);
			Assert::AreEqual(cycles, 20);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_D8_NotCarry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11100000;
			int cycles = ops.op_D8(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_D9()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			interrupts.DisableInterrupts();
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_D9(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(interrupts.InterruptsEnabled(), true);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_DA_Carry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_DA(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_DA_NotCarry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11100000;
			int cycles = ops.op_DA(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_DC_Carry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_DC(&registers);
			Assert::AreEqual(cycles, 24);
			Assert::AreEqual(registers.pc, 0xde37);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_DC_NotCarry()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11100000;
			int cycles = ops.op_DC(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11100000);
		};

		[TestMethod]
		void Opcode_DE_NoZero_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x01, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x12;
			registers.f = 0b10110000;
			int cycles = ops.op_DE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01000000);
		};

		[TestMethod]
		void Opcode_DE_NoZero_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x05, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x15;
			registers.f = 0b10010000;
			int cycles = ops.op_DE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x0f);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_DE_NoZero_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x80, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x00;
			registers.f = 0b10100000;
			int cycles = ops.op_DE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x80);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_DE_NoZero_Carry_HalfCarry()
		{
			MockMMU mmu(0x41, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x20;
			registers.f = 0b11000000;
			int cycles = ops.op_DE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0xdf);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_DE_Zero()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x11;
			registers.f = 0b00110000;
			int cycles = ops.op_DE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_DF()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_DF(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x18);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_E0()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_E0(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: make sure WriteByte() was called correctly
		};

		[TestMethod]
		void Opcode_E1()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.h = 1;
			registers.l = 2;
			registers.f = 0b11110000;
			int cycles = ops.op_E1(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.h, 0x4c);
			Assert::AreEqual(registers.l, 0x4c);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_E2()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.a = 10;
			registers.c = 4;
			registers.f = 0b11110000;
			int cycles = ops.op_E2(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: make sure WriteByte() was called correctly
		};

		[TestMethod]
		void Opcode_E5()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.h = 0x05;
			registers.l = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_E5(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.h, 0x05);
			Assert::AreEqual(registers.l, 0x10);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_E6_Zero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.f = 0b01010000;
			int cycles = ops.op_E6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10100000);
		};

		[TestMethod]
		void Opcode_E6_NonZero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11010000;
			int cycles = ops.op_E6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0b00010000);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_E7()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_E7(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x20);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_E8_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd000;
			registers.f = 0b11110000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xd04c);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_E8_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x30, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd0e0;
			registers.f = 0b11100000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xd110);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_E8_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x04, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd00c;
			registers.f = 0b11010000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xd010);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_E8_Carry_HalfCarry()
		{
			MockMMU mmu(0x01, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd0ff;
			registers.f = 0b11000000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xd100);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_E8_Subtract_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0xf0, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd0ff;
			registers.f = 0b11110000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xd0ef);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_E8_Subtract_Carry_NoHalfCarry()
		{
			MockMMU mmu(0xb0, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd040;
			registers.f = 0b11100000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xcff0);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_E8_Subtract_NoCarry_HalfCarry()
		{
			MockMMU mmu(0xf3, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd01c;
			registers.f = 0b11010000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xd00f);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_E8_Subtract_Carry_HalfCarry()
		{
			MockMMU mmu(0xff, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd000;
			registers.f = 0b11000000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0xcfff);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_E8_Overflow()
		{
			MockMMU mmu(0x7f, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xff81;
			registers.f = 0b11100000;
			int cycles = ops.op_E8(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 0x0000);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_E9()
		{
			MockMMU mmu(0xff, 0x4190);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0x40;
			registers.l = 0x50;
			registers.f = 0b11110000;
			int cycles = ops.op_E9(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 0x4190);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_EA()
		{
			MockMMU mmu(0xff, 0x4190);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_EA(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_EE_Zero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b11110000;
			registers.f = 0b01110000;
			int cycles = ops.op_EE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_EE_NonZero()
		{
			MockMMU mmu(0b11110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11110000;
			int cycles = ops.op_EE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0b11101100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_EF()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_EF(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x28);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_F0()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_F0(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.a, 0xef);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_F1()
		{
			MockMMU mmu(0xc0, 0x7fc0);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.a = 1;
			registers.f = 0b00000001;
			int cycles = ops.op_F1(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 32);
			Assert::AreEqual(registers.a, 0xc0);
			Assert::AreEqual(registers.f, 0xc0);
		};

		[TestMethod]
		void Opcode_F2()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.a = 10;
			registers.c = 4;
			registers.f = 0b11110000;
			int cycles = ops.op_F2(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(registers.a, 0xef);
			Assert::AreEqual(registers.f, 0b11110000);
			// TODO: make sure WriteByte() was called correctly
		};

		[TestMethod]
		void Opcode_F3()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_F3(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(interrupts.InterruptsEnabled(), false);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_F5()
		{
			MockMMU mmu(0x4c, 0xde37);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.a = 0x05;
			registers.f = 0xf0;
			int cycles = ops.op_F5(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.a, 0x05);
			Assert::AreEqual(registers.f, 0xf0);
		};

		[TestMethod]
		void Opcode_F6_Zero()
		{
			MockMMU mmu(0b00000000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00000000;
			registers.f = 0b01110000;
			int cycles = ops.op_F6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x00);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_F6_NonZero()
		{
			MockMMU mmu(0b10110000, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00011100;
			registers.f = 0b11110000;
			int cycles = ops.op_F6(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0b10111100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_F7()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_F7(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x30);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_F8_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0x4c, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd000;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11110000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xd0);
			Assert::AreEqual(registers.l, 0x4c);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_F8_Carry_NoHalfCarry()
		{
			MockMMU mmu(0x30, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd0e0;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11100000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xd1);
			Assert::AreEqual(registers.l, 0x10);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_F8_NoCarry_HalfCarry()
		{
			MockMMU mmu(0x04, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd00c;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11010000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xd0);
			Assert::AreEqual(registers.l, 0x10);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_F8_Carry_HalfCarry()
		{
			MockMMU mmu(0x01, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd0ff;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11000000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xd1);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_F8_Subtract_NoCarry_NoHalfCarry()
		{
			MockMMU mmu(0xf0, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd0ff;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11110000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xd0);
			Assert::AreEqual(registers.l, 0xef);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_F8_Subtract_Carry_NoHalfCarry()
		{
			MockMMU mmu(0xb0, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd040;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11100000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xcf);
			Assert::AreEqual(registers.l, 0xf0);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_F8_Subtract_NoCarry_HalfCarry()
		{
			MockMMU mmu(0xf3, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd01c;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11010000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xd0);
			Assert::AreEqual(registers.l, 0x0f);
			Assert::AreEqual(registers.f, 0b00100000);
		};

		[TestMethod]
		void Opcode_F8_Subtract_Carry_HalfCarry()
		{
			MockMMU mmu(0xff, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xd000;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11000000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0xcf);
			Assert::AreEqual(registers.l, 0xff);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_F8_Overflow()
		{
			MockMMU mmu(0x7f, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xff81;
			registers.h = 10;
			registers.l = 20;
			registers.f = 0b11100000;
			int cycles = ops.op_F8(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.h, 0x00);
			Assert::AreEqual(registers.l, 0x00);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_F9()
		{
			MockMMU mmu(0x7f, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 0xff81;
			registers.h = 0x56;
			registers.l = 0x10;
			registers.f = 0b11110000;
			int cycles = ops.op_F9(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0x56);
			Assert::AreEqual(registers.l, 0x10);
			Assert::AreEqual(registers.sp, 0x5610);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_FA()
		{
			MockMMU mmu(0xf0, 0x4190);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b11110000;
			int cycles = ops.op_FA(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 12);
			Assert::AreEqual(registers.a, 0xf0);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_FB()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			interrupts.DisableInterrupts();
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_FB(&registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.sp, 30);
			Assert::AreEqual(interrupts.InterruptsEnabled(), true);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_FE_Zero()
		{
			MockMMU mmu(0x10, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b00110000;
			int cycles = ops.op_FE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b11000000);
		};

		[TestMethod]
		void Opcode_FE_HalfCarry()
		{
			MockMMU mmu(0x05, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b10010000;
			int cycles = ops.op_FE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01100000);
		};

		[TestMethod]
		void Opcode_FE_Carry()
		{
			MockMMU mmu(0x20, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b10100000;
			int cycles = ops.op_FE(&registers);
			Assert::AreEqual(cycles, 8);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0x10);
			Assert::AreEqual(registers.f, 0b01010000);
		};

		[TestMethod]
		void Opcode_FF()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.op_FF(&registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x38);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_RLC_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b00011001;
			registers.f = 0b11110000;
			ops.op_CB_RLC(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b00110010);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_RLC_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b10011001;
			registers.f = 0b11100000;
			ops.op_CB_RLC(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b00110011);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_RLC_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00000000;
			registers.f = 0b01110000;
			ops.op_CB_RLC(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_CB_RRC_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b10011010;
			registers.f = 0b11110000;
			ops.op_CB_RRC(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b01001101);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_RRC_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b10011001;
			registers.f = 0b11100000;
			ops.op_CB_RRC(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b11001100);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_RRC_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00000000;
			registers.f = 0b01110000;
			ops.op_CB_RRC(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_CB_RL_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b10011010;
			registers.f = 0b11100000;
			ops.op_CB_RL(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b00110100);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_RL_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b00011001;
			registers.f = 0b11110000;
			ops.op_CB_RL(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b00110011);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_RL_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b10000000;
			registers.f = 0b01100000;
			ops.op_CB_RL(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_CB_RR_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b10011010;
			registers.f = 0b11110000;
			ops.op_CB_RR(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b11001101);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_RR_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b10011001;
			registers.f = 0b11100000;
			ops.op_CB_RR(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b01001100);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_RR_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00000001;
			registers.f = 0b01100000;
			ops.op_CB_RR(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_CB_SLA_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b01011010;
			registers.f = 0b11110000;
			ops.op_CB_SLA(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b10110100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_SLA_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b10011001;
			registers.f = 0b11100000;
			ops.op_CB_SLA(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b00110010);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_SLA_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b10000000;
			registers.f = 0b01100000;
			ops.op_CB_SLA(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_CB_SRA_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b10011010;
			registers.f = 0b11110000;
			ops.op_CB_SRA(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b11001101);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_SRA_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b10011001;
			registers.f = 0b11100000;
			ops.op_CB_SRA(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b11001100);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_SRA_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00000001;
			registers.f = 0b01100000;
			ops.op_CB_SRA(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_CB_SWAP_NotZero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b11000101;
			registers.f = 0b11110000;
			ops.op_CB_SWAP(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b01011100);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_SWAP_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0b00000000;
			registers.f = 0b01110000;
			ops.op_CB_SWAP(&registers, &registers.e);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0b00000000);
			Assert::AreEqual(registers.f, 0b10000000);
		};

		[TestMethod]
		void Opcode_CB_SRL_NoCarry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b10011010;
			registers.f = 0b11110000;
			ops.op_CB_SRL(&registers, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b01001101);
			Assert::AreEqual(registers.f, 0b00000000);
		};

		[TestMethod]
		void Opcode_CB_SRL_Carry()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.c = 0b10011001;
			registers.f = 0b11100000;
			ops.op_CB_SRL(&registers, &registers.c);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.c, 0b01001100);
			Assert::AreEqual(registers.f, 0b00010000);
		};

		[TestMethod]
		void Opcode_CB_SRL_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00000001;
			registers.f = 0b01100000;
			ops.op_CB_SRL(&registers, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00000000);
			Assert::AreEqual(registers.f, 0b10010000);
		};

		[TestMethod]
		void Opcode_CB_BIT_Zero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00001000;
			registers.f = 0b01010000;
			ops.op_CB_BIT(&registers, 2, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00001000);
			Assert::AreEqual(registers.f, 0b10110000);
		};

		[TestMethod]
		void Opcode_CB_BIT_NotZero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0b00101000;
			registers.f = 0b11010000;
			ops.op_CB_BIT(&registers, 5, &registers.e);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0b00101000);
			Assert::AreEqual(registers.f, 0b00110000);
		};

		[TestMethod]
		void Opcode_CB_RES_BitZero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0b00101001;
			registers.f = 0b11110000;
			ops.op_CB_RES(&registers, 0, &registers.e);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0b00101000);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_RES_BitZero_Same()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00101010;
			registers.f = 0b11110000;
			ops.op_CB_RES(&registers, 0, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00101010);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_RES_BitSeven()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.e = 0b10101001;
			registers.f = 0b11110000;
			ops.op_CB_RES(&registers, 7, &registers.e);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.e, 0b00101001);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_RES_BitSeven_Same()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00101010;
			registers.f = 0b11110000;
			ops.op_CB_RES(&registers, 7, &registers.a);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b00101010);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_SET_BitZero()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.h = 0b00101000;
			registers.f = 0b11110000;
			ops.op_CB_SET(&registers, 0, &registers.h);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.h, 0b00101001);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_SET_BitZero_Same()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.d = 0b00101011;
			registers.f = 0b11110000;
			ops.op_CB_SET(&registers, 0, &registers.d);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.d, 0b00101011);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_SET_BitSeven()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b00101001;
			registers.f = 0b11110000;
			ops.op_CB_SET(&registers, 7, &registers.b);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.b, 0b10101001);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB_SET_BitSeven_Same()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b10101010;
			registers.f = 0b11110000;
			ops.op_CB_SET(&registers, 7, &registers.a);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.a, 0b10101010);
			Assert::AreEqual(registers.f, 0b11110000);
		};

		[TestMethod]
		void Opcode_CB00()
		{
			MockMMU mmu(0x00, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.b = 0b00011001;
			registers.f = 0b11110000;
			ops.op_CB(&registers);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.b, 0b00110010);
			Assert::AreEqual(registers.f, 0b00000000);
		}

		[TestMethod]
		void Opcode_CBFF()
		{
			MockMMU mmu(0xff, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.a = 0b00101001;
			registers.f = 0b11110000;
			ops.op_CB(&registers);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.a, 0b10101001);
			Assert::AreEqual(registers.f, 0b11110000);
		}

		[TestMethod]
		void ExecuteInstruction_Opcode_0()
		{
			MockMMU mmu;
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b00000000;
			int cycles = ops.ExecuteInstruction(0x00, &registers);
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 10);
			Assert::AreEqual(registers.f, 0b00000000);
		}

		[TestMethod]
		void ExecuteInstruction_Opcode_FF()
		{
			MockMMU mmu(0xef, 0xa734);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			registers.pc = 10;
			registers.sp = 30;
			registers.f = 0b11110000;
			int cycles = ops.ExecuteInstruction(0xFF, &registers);
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 0x38);
			Assert::AreEqual(registers.sp, 28);
			Assert::AreEqual(registers.f, 0b11110000);
		}
	};
}
