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
			CPUOpcodes ops (&mmu);
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
			MockMMU mmu (0x4c, 0xa734);
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
			CPURegisters registers;
			registers.pc = 10;
			registers.f = 0b01110000;
			int cycles = ops.op_20(&registers);
			Assert::AreEqual(cycles, 12);
			Assert::AreEqual(registers.pc, 9);
			Assert::AreEqual(registers.f, 0b01110000);
		};

		[TestMethod]
		void Opcode_21_NoZeroFlag_JumpForward()
		{
			MockMMU mmu(0x5, 0xa734);
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
			CPUOpcodes ops(&mmu);
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
	};
}
