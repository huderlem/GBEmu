#include "stdafx.h"

#include "../GameBoyEmulator/CPU.h"
#pragma comment(lib, "../Debug/GameBoyEmulator.lib")

#include "MockMMU.h"

using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
using namespace	Microsoft::VisualStudio::TestTools::UnitTesting;

namespace GameBoyEmulatorTest
{
	[TestClass]
	public ref class CPUTest
	{
	public: 
		[TestMethod]
		void ExecuteNextInstruction_00()
		{
			MockMMU mmu(0x00, 0xd000);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			CPU cpu(&mmu, &registers, &ops);
			registers.pc = 10;
			registers.f = 0b00000000;
			int cycles = cpu.ExecuteNextInstruction();
			Assert::AreEqual(cycles, 4);
			Assert::AreEqual(registers.pc, 11);
			Assert::AreEqual(registers.f, 0b00000000);
		}

		[TestMethod]
		void ExecuteNextInstruction_FA()
		{
			MockMMU mmu(0xFA, 0x4567);
			Interrupts interrupts;
			CPUOpcodes ops(&mmu, &interrupts);
			CPURegisters registers;
			CPU cpu(&mmu, &registers, &ops);
			registers.pc = 10;
			registers.a = 0x10;
			registers.f = 0b00110000;
			int cycles = cpu.ExecuteNextInstruction();
			Assert::AreEqual(cycles, 16);
			Assert::AreEqual(registers.pc, 13);
			Assert::AreEqual(registers.a, 0xFA);
			Assert::AreEqual(registers.f, 0b00110000);
		}
	};
}
