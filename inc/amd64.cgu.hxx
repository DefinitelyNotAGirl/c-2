/*
 * Created Date: Thursday June 6th 2024
 * Author: Lilith
 * -----
 * Last Modified: Thursday June 6th 2024 5:58:25 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <compiler.h>

namespace amd64
{
	/**
	 * @brief each value in this enum really only has 2 bits, the upper 6 are to be ignored
	 */
	enum class AddressingMode : uint8_t {
		RegisterDirect = 0b11,
		RegisterIndirect = 0b00,
		IndexedRegisterIndirect = 0b00
	};

	/**
	 * @brief each value in this enum really only has 3 bits, the upper 5 are to be ignored
	 */
	enum class Register : uint8_t {
		
	};

	uint64_t imm64(uint64_t value);
	uint64_t imm32(uint32_t value);
	uint64_t imm16(uint16_t value);
	/**
	 * @brief this is short hand for modRM(reg,AddressingMode::RegisterDirect,rm)
	 * 
	 * @param reg reg operrand
	 * @param rm reg/mem opperand
	 * @return amd64 modRM byte ready for execution
	 */
	byte modRM(Register reg,Register rm);
	/**
	 * @brief
	 * 
	 * @param reg reg operrand
	 * @param rm reg/mem opperand
	 * @return amd64 modRM byte ready for execution
	 */
	byte modRM(Register reg,AddressingMode mod,Register rm);
	/**
	 * @brief
	 * 
	 * @param scale must be either 1,2,4 or 8 
	 * @param index register holding the index
	 * @param base register holding the base address
	 * @return amd64 SIB byte ready for execution
	 */
	byte SIB(uint8_t scale, Register index, Register base);

	namespace prefix
	{
		/**
		 * @brief constructs a REX prefix
		 * 
		 * @param W
		 * @param R
		 * @param X
		 * @param B
		 * @return amd64 REX byte ready for execution
		 */
		byte REX(bool W, bool R, bool X, bool B);
		/**
		 * @brief initiates a 2 byte VEX escape sequence
		 */
		byte VEX_2byte = 0xC5;
		/**
		 * @brief initiates a 3 byte VEX escape sequence
		 */
		byte VEX_3byte = 0xC4;
		/**
		 * @brief initiates a 3 byte XOP escape sequence
		 */
		byte XOP = 0x8F;
		namespace legacy
		{
			/**
			 * @brief AMD64 specification: 
			 `
			 	In 64-bit mode, the prefix allows mixing of 16-bit, 32-bit, and 64-bit data on an instruction-byinstruction basis.
			 `
			 ... 
			 `
			 	In 64-bit mode, most instructions default to a 32-bit operand size. For these instructions, a REX prefix
			 	can specify a 64-bit operand size, and a 66h prefix specifies a 16-bit operand size. The REX
			 	prefix takes precedence over the 66h prefix. However, if an instruction defaults to a 64-bit operand
			 	size, it does not need a REX prefix and it can only be overridden to a 16-bit operand size. It cannot be
			 	overridden to a 32-bit operand size, because there is no 32-bit operand-size override prefix in 64-bit
			 	mode.
			 `
			 ...
			 `
			 	Instructions that Cannot Use the Operand-Size Prefix: The operand-size prefix should be used
			 	only with general-purpose instructions and the x87 FLDENV, FNSTENV, FNSAVE, and FRSTOR
			 	instructions, in which the prefix selects between 16-bit and 32-bit operand size. The prefix is ignored
			 	by all other x87 instructions and by 64-bit media floating-point (3DNow!™) instructions.
			 	For other instructions (mostly SIMD instructions) the 66h, F2h, and F3h prefixes are used as opcode
			 	extensions to extend the instruction encoding space in the 0Fh, 0F_38h, and 0F_3Ah opcode maps.
			 `
			 */
			byte OperandSizeOverride = 0x66;
			/**
			 * @brief 
			 * 
			 */
			byte AddressSizeOverride = 0x67;
			/**
			 * @brief AMD64 specification:
			 `
			 	Segment Overrides in 64-Bit Mode: 
				In 64-bit mode, the CS, DS, ES, and SS segment-override
				prefixes have no effect. These four prefixes are not treated as segment-override prefixes for the
				purposes of multiple-prefix rules. Instead, they are treated as null prefixes.
				The FS and GS segment-override prefixes are treated as true segment-override prefixes in 64-bit
				mode. Use of the FS or GS prefix causes their respective segment bases to be added to the effective
				address calculation. See “FS and GS Registers in 64-Bit Mode” in APM Volume 2 for details. 
			 `
			 */
			namespace SegmentOverride
			{
				byte cs = 0x2E;
				byte ds = 0x3E;
				byte es = 0x26;
				byte fs = 0x64;
				byte gs = 0x65;
				byte ss = 0x36;
			}
			/**
			 * @brief AMD64 specification:
			 `
			 	The LOCK prefix causes certain kinds of memory read-modify-write instructions to occur atomically.
			 `
			 ...
			 `
				The prefix is intended to give the processor exclusive use of shared memory in a multiprocessor system.
			 `
			 ...
			 `
			 	The LOCK prefix can only be used with forms of the following instructions that write a memory
				operand: ADC, ADD, AND, BTC, BTR, BTS, CMPXCHG, CMPXCHG8B, CMPXCHG16B, DEC,
				INC, NEG, NOT, OR, SBB, SUB, XADD, XCHG, and XOR. An invalid-opcode exception occurs if
				the LOCK prefix is used with any other instruction. 
			 `
			 */
			byte LOCK = 0xF0;
			/**
			 * @brief AMD64 specification:
			 `
			 	The repeat prefixes cause repetition of certain instructions that load, store, move, input, or output
				strings. The prefixes should only be used with such string instructions. Two pairs of repeat prefixes,
				REPE/REPZ and REPNE/REPNZ, perform the same repeat functions for certain compare-string and
				scan-string instructions. The repeat function uses rCX as a count register. The size of rCX is based on
				address size,
			 `
			 ...
			 `
			 	The REP prefix repeats its associated string instruction the number of times specified in the
				counter register (rCX). It terminates the repetition when the value in rCX reaches 0. The prefix can be
				used with the INS, LODS, MOVS, OUTS, and STOS instructions.
			 `
			 */
			byte REP  = 0xF3;
			/**
			 * @brief AMD64 specification:
			 `
			 	The repeat prefixes cause repetition of certain instructions that load, store, move, input, or output
				strings. The prefixes should only be used with such string instructions. Two pairs of repeat prefixes,
				REPE/REPZ and REPNE/REPNZ, perform the same repeat functions for certain compare-string and
				scan-string instructions. The repeat function uses rCX as a count register. The size of rCX is based on
				address size,
			 `
			 ...
			 `
			 	REPE and REPZ are synonyms and have identical opcodes. These prefixes repeat
				their associated string instruction the number of times specified in the counter register (rCX). The
				repetition terminates when the value in rCX reaches 0 or when the zero flag (ZF) is cleared to 0. The
				REPE and REPZ prefixes can be used with the CMPS, CMPSB, CMPSD, CMPSW, SCAS, SCASB,
				SCASD, and SCASW instructions.
			 `
			 */
			byte REPE = 0xF3;
			/**
			 * @brief AMD64 specification:
			 `
			 	The repeat prefixes cause repetition of certain instructions that load, store, move, input, or output
				strings. The prefixes should only be used with such string instructions. Two pairs of repeat prefixes,
				REPE/REPZ and REPNE/REPNZ, perform the same repeat functions for certain compare-string and
				scan-string instructions. The repeat function uses rCX as a count register. The size of rCX is based on
				address size,
			 `
			 ...
			 `
			 	REPE and REPZ are synonyms and have identical opcodes. These prefixes repeat
				their associated string instruction the number of times specified in the counter register (rCX). The
				repetition terminates when the value in rCX reaches 0 or when the zero flag (ZF) is cleared to 0. The
				REPE and REPZ prefixes can be used with the CMPS, CMPSB, CMPSD, CMPSW, SCAS, SCASB,
				SCASD, and SCASW instructions.
			 `
			 */
			byte REPZ = 0xF3;
			/**
			 * @brief AMD64 specification:
			 `
			 	The repeat prefixes cause repetition of certain instructions that load, store, move, input, or output
				strings. The prefixes should only be used with such string instructions. Two pairs of repeat prefixes,
				REPE/REPZ and REPNE/REPNZ, perform the same repeat functions for certain compare-string and
				scan-string instructions. The repeat function uses rCX as a count register. The size of rCX is based on
				address size,
			 `
			 ...
			 `
			 	REPNE and REPNZ are synonyms and have identical opcodes. These prefixes
				repeat their associated string instruction the number of times specified in the counter register (rCX).
				The repetition terminates when the value in rCX reaches 0 or when the zero flag (ZF) is set to 1. The
				REPNE and REPNZ prefixes can be used with the CMPS, CMPSB, CMPSD, CMPSW, SCAS,
				SCASB, SCASD, and SCASW instructions.
			 `
			 */
			byte REPNE= 0xF2;
			/**
			 * @brief AMD64 specification:
			 `
			 	The repeat prefixes cause repetition of certain instructions that load, store, move, input, or output
				strings. The prefixes should only be used with such string instructions. Two pairs of repeat prefixes,
				REPE/REPZ and REPNE/REPNZ, perform the same repeat functions for certain compare-string and
				scan-string instructions. The repeat function uses rCX as a count register. The size of rCX is based on
				address size,
			 `
			 ...
			 `
			 	REPNE and REPNZ are synonyms and have identical opcodes. These prefixes
				repeat their associated string instruction the number of times specified in the counter register (rCX).
				The repetition terminates when the value in rCX reaches 0 or when the zero flag (ZF) is set to 1. The
				REPNE and REPNZ prefixes can be used with the CMPS, CMPSB, CMPSD, CMPSW, SCAS,
				SCASB, SCASD, and SCASW instructions.
			 `
			 */
			byte REPNZ= 0xF2;
		}
	}

	namespace opcode
	{
	}
}
