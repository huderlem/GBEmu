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
			Assert::AreEqual(registers.pc, 12);
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
	};
}
