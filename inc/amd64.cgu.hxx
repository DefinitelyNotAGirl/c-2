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
		namespace add{
			constexpr byte r_m8__r8 = 0x00;
			constexpr byte r_m16_32_64__r16_32_64 = 0x01;
			constexpr byte r8__r_m8 = 0x02;
			constexpr byte r16_32_64__r_m16_32_64 = 0x03;
			constexpr byte AL__imm8 = 0x04;
			constexpr byte rAX__imm16_32 = 0x05;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace or{
			constexpr byte r_m8__r8 = 0x08;
			constexpr byte r_m16_32_64__r16_32_64 = 0x09;
			constexpr byte r8__r_m8 = 0x0A;
			constexpr byte r16_32_64__r_m16_32_64 = 0x0B;
			constexpr byte AL__imm8 = 0x0C;
			constexpr byte rAX__imm16_32 = 0x0D;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace adc{
			constexpr byte r_m8__r8 = 0x10;
			constexpr byte r_m16_32_64__r16_32_64 = 0x11;
			constexpr byte r8__r_m8 = 0x12;
			constexpr byte r16_32_64__r_m16_32_64 = 0x13;
			constexpr byte AL__imm8 = 0x14;
			constexpr byte rAX__imm16_32 = 0x15;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace sbb{
			constexpr byte r_m8__r8 = 0x18;
			constexpr byte r_m16_32_64__r16_32_64 = 0x19;
			constexpr byte r8__r_m8 = 0x1A;
			constexpr byte r16_32_64__r_m16_32_64 = 0x1B;
			constexpr byte AL__imm8 = 0x1C;
			constexpr byte rAX__imm16_32 = 0x1D;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace and{
			constexpr byte r_m8__r8 = 0x20;
			constexpr byte r_m16_32_64__r16_32_64 = 0x21;
			constexpr byte r8__r_m8 = 0x22;
			constexpr byte r16_32_64__r_m16_32_64 = 0x23;
			constexpr byte AL__imm8 = 0x24;
			constexpr byte rAX__imm16_32 = 0x25;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace sub{
			constexpr byte r_m8__r8 = 0x28;
			constexpr byte r_m16_32_64__r16_32_64 = 0x29;
			constexpr byte r8__r_m8 = 0x2A;
			constexpr byte r16_32_64__r_m16_32_64 = 0x2B;
			constexpr byte AL__imm8 = 0x2C;
			constexpr byte rAX__imm16_32 = 0x2D;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace xor{
			constexpr byte r_m8__r8 = 0x30;
			constexpr byte r_m16_32_64__r16_32_64 = 0x31;
			constexpr byte r8__r_m8 = 0x32;
			constexpr byte r16_32_64__r_m16_32_64 = 0x33;
			constexpr byte AL__imm8 = 0x34;
			constexpr byte rAX__imm16_32 = 0x35;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace cmp{
			constexpr byte r_m8__r8 = 0x38;
			constexpr byte r_m16_32_64__r16_32_64 = 0x39;
			constexpr byte r8__r_m8 = 0x3A;
			constexpr byte r16_32_64__r_m16_32_64 = 0x3B;
			constexpr byte AL__imm8 = 0x3C;
			constexpr byte rAX__imm16_32 = 0x3D;
			constexpr byte r_m8__imm8 = 0x80;
			constexpr byte r_m16_32_64__imm16_32 = 0x81;
			constexpr byte r_m16_32_64__imm8 = 0x83;
		}
		namespace movsxd{
			constexpr byte r32_64__r_m32 = 0x63;
		}
		namespace push{
			constexpr byte imm16_32 = 0x68;
			constexpr byte imm8 = 0x6A;
			constexpr byte r_m16_32 = 0xFF;
			constexpr byte r_m64_16 = 0xFF;
		}
		namespace imul{
			constexpr byte r16_32_64__r_m16_32_64__imm16_32 = 0x69;
			constexpr byte r16_32_64__r_m16_32_64__imm8 = 0x6B;
			constexpr byte AX__AL__r_m8 = 0xF6;
			constexpr byte rDX__rAX__r_m16_32_64 = 0xF7;
		}
		namespace ins{
			constexpr byte m8__DX = 0x6C;
			constexpr byte m16__DX = 0x6D;
			constexpr byte m16_32__DX = 0x6D;
		}
		namespace outs{
			constexpr byte DX__m8 = 0x6E;
			constexpr byte DX__m16 = 0x6F;
			constexpr byte DX__m16_32 = 0x6F;
		}
		namespace jo{
			constexpr byte rel8 = 0x70;
		}
		namespace jno{
			constexpr byte rel8 = 0x71;
		}
		namespace jb{
			constexpr byte rel8 = 0x72;
		}
		namespace jnb{
			constexpr byte rel8 = 0x73;
		}
		namespace jz{
			constexpr byte rel8 = 0x74;
		}
		namespace jnz{
			constexpr byte rel8 = 0x75;
		}
		namespace jbe{
			constexpr byte rel8 = 0x76;
		}
		namespace jnbe{
			constexpr byte rel8 = 0x77;
		}
		namespace js{
			constexpr byte rel8 = 0x78;
		}
		namespace jns{
			constexpr byte rel8 = 0x79;
		}
		namespace jp{
			constexpr byte rel8 = 0x7A;
		}
		namespace jnp{
			constexpr byte rel8 = 0x7B;
		}
		namespace jl{
			constexpr byte rel8 = 0x7C;
		}
		namespace jnl{
			constexpr byte rel8 = 0x7D;
		}
		namespace jle{
			constexpr byte rel8 = 0x7E;
		}
		namespace jnle{
			constexpr byte rel8 = 0x7F;
		}
		namespace test{
			constexpr byte r_m8__r8 = 0x84;
			constexpr byte r_m16_32_64__r16_32_64 = 0x85;
			constexpr byte AL__imm8 = 0xA8;
			constexpr byte rAX__imm16_32 = 0xA9;
			constexpr byte r_m8__imm8 = 0xF6;
			constexpr byte r_m8__imm8 = 0xF6;
			constexpr byte r_m16_32_64__imm16_32 = 0xF7;
			constexpr byte r_m16_32_64__imm16_32 = 0xF7;
		}
		namespace xchg{
			constexpr byte r8__r_m8 = 0x86;
			constexpr byte r16_32_64__r_m16_32_64 = 0x87;
		}
		namespace mov{
			constexpr byte r_m8__r8 = 0x88;
			constexpr byte r_m16_32_64__r16_32_64 = 0x89;
			constexpr byte r8__r_m8 = 0x8A;
			constexpr byte r16_32_64__r_m16_32_64 = 0x8B;
			constexpr byte m16__Sreg = 0x8C;
			constexpr byte Sreg__r_m16 = 0x8E;
			constexpr byte AL__moffs8 = 0xA0;
			constexpr byte rAX__moffs16_32_64 = 0xA1;
			constexpr byte moffs8__AL = 0xA2;
			constexpr byte moffs16_32_64__rAX = 0xA3;
			constexpr byte r_m8__imm8 = 0xC6;
			constexpr byte r_m16_32_64__imm16_32 = 0xC7;
		}
		namespace lea{
			constexpr byte r16_32_64__m = 0x8D;
		}
		namespace pop{
			constexpr byte r_m16_32 = 0x8F;
			constexpr byte r_m64_16 = 0x8F;
		}
		namespace r16_32_64{
			constexpr byte rAX = 0x90+r;
			constexpr byte imm16_32_64 = 0xB8+r;
		}
		namespace nop{
			constexpr byte _ = 0x90;
		}
		namespace pause{
			constexpr byte _ = 0x90;
		}
		namespace cbw{
			constexpr byte AX__AL = 0x98;
		}
		namespace cwd{
			constexpr byte DX__AX = 0x99;
		}
		namespace fwait{
			constexpr byte _ = 0x9B;
		}
		namespace pushf{
			constexpr byte Flags = 0x9C;
		}
		namespace popf{
			constexpr byte Flags = 0x9D;
		}
		namespace sahf{
			constexpr byte AH = 0x9E;
		}
		namespace lahf{
			constexpr byte AH = 0x9F;
		}
		namespace movs{
			constexpr byte m8__m8 = 0xA4;
			constexpr byte m16_32_64__m16_32_64 = 0xA5;
		}
		namespace cmps{
			constexpr byte m8__m8 = 0xA6;
			constexpr byte m16_32_64__m16_32_64 = 0xA7;
		}
		namespace stos{
			constexpr byte m8__AL = 0xAA;
			constexpr byte m16_32_64__rAX = 0xAB;
		}
		namespace lods{
			constexpr byte AL__m8 = 0xAC;
			constexpr byte rAX__m16_32_64 = 0xAD;
		}
		namespace scas{
			constexpr byte m8__AL = 0xAE;
			constexpr byte m16_32_64__rAX = 0xAF;
		}
		namespace rol{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace ror{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace rcl{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace rcr{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace shl{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace shr{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace sal{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace sar{
			constexpr byte r_m8__imm8 = 0xC0;
			constexpr byte r_m16_32_64__imm8 = 0xC1;
			constexpr byte r_m8__1 = 0xD0;
			constexpr byte r_m16_32_64__1 = 0xD1;
			constexpr byte r_m8__CL = 0xD2;
			constexpr byte r_m16_32_64__CL = 0xD3;
		}
		namespace retn{
			constexpr byte imm16 = 0xC2;
			constexpr byte _ = 0xC3;
		}
		namespace enter{
			constexpr byte rBP__imm16__imm8 = 0xC8;
		}
		namespace leave{
			constexpr byte rBP = 0xC9;
		}
		namespace retf{
			constexpr byte imm16 = 0xCA;
			constexpr byte _ = 0xCB;
		}
		namespace int{
			constexpr byte 3__eFlags = 0xCC;
			constexpr byte imm8__eFlags = 0xCD;
		}
		namespace into{
			constexpr byte eFlags = 0xCE;
		}
		namespace iret{
			constexpr byte Flags = 0xCF;
		}
		namespace xlat{
			constexpr byte AL__m8 = 0xD7;
		}
		namespace fadd{
			constexpr byte ST__m32real = 0xD8;
			constexpr byte ST__m64real = 0xDC;
			constexpr byte STi__ST = 0xDC;
		}
		namespace fmul{
			constexpr byte ST__m32real = 0xD8;
			constexpr byte ST__m64real = 0xDC;
			constexpr byte STi__ST = 0xDC;
		}
		namespace fcom{
			constexpr byte ST__STi_m32real = 0xD8;
			constexpr byte ST__ST1 = 0xD8;
			constexpr byte ST__m64real = 0xDC;
		}
		namespace fcomp{
			constexpr byte ST__STi_m32real = 0xD8;
			constexpr byte ST__ST1 = 0xD8;
			constexpr byte ST__m64real = 0xDC;
		}
		namespace fsub{
			constexpr byte ST__m32real = 0xD8;
			constexpr byte ST__m64real = 0xDC;
			constexpr byte STi__ST = 0xDC;
		}
		namespace fsubr{
			constexpr byte ST__m32real = 0xD8;
			constexpr byte STi__ST = 0xDC;
			constexpr byte ST__m64real = 0xDC;
		}
		namespace fdiv{
			constexpr byte ST__m32real = 0xD8;
			constexpr byte ST__m64real = 0xDC;
			constexpr byte STi__ST = 0xDC;
		}
		namespace fdivr{
			constexpr byte ST__m32real = 0xD8;
			constexpr byte STi__ST = 0xDC;
			constexpr byte ST__m64real = 0xDC;
		}
		namespace fld{
			constexpr byte ST__STi_m32real = 0xD9;
			constexpr byte ST__m80real = 0xDB;
			constexpr byte ST__m64real = 0xDD;
		}
		namespace fxch{
			constexpr byte ST__STi = 0xD9;
			constexpr byte ST__ST1 = 0xD9;
		}
		namespace fst{
			constexpr byte m32real__ST = 0xD9;
			constexpr byte m64real__ST = 0xDD;
			constexpr byte ST__STi = 0xDD;
		}
		namespace fnop{
			constexpr byte _ = 0xD9;
		}
		namespace fstp{
			constexpr byte m32real__ST = 0xD9;
			constexpr byte m80real__ST = 0xDB;
			constexpr byte m64real__ST = 0xDD;
			constexpr byte ST__STi = 0xDD;
		}
		namespace fstp1{
			constexpr byte STi__ST = 0xD9;
		}
		namespace fldenv{
			constexpr byte m14_28 = 0xD9;
		}
		namespace fchs{
			constexpr byte ST = 0xD9;
		}
		namespace fabs{
			constexpr byte ST = 0xD9;
		}
		namespace ftst{
			constexpr byte ST = 0xD9;
		}
		namespace fxam{
			constexpr byte ST = 0xD9;
		}
		namespace fldcw{
			constexpr byte m16 = 0xD9;
		}
		namespace fld1{
			constexpr byte ST = 0xD9;
		}
		namespace fldl2t{
			constexpr byte ST = 0xD9;
		}
		namespace fldl2e{
			constexpr byte ST = 0xD9;
		}
		namespace fldpi{
			constexpr byte ST = 0xD9;
		}
		namespace fldlg2{
			constexpr byte ST = 0xD9;
		}
		namespace fldln2{
			constexpr byte ST = 0xD9;
		}
		namespace fldz{
			constexpr byte ST = 0xD9;
		}
		namespace fnstenv{
			constexpr byte m14_28 = 0xD9;
		}
		namespace fstenv{
			constexpr byte m14_28 = 0xD9;
		}
		namespace f2xm1{
			constexpr byte ST = 0xD9;
		}
		namespace fyl2x{
			constexpr byte ST1__ST = 0xD9;
		}
		namespace fptan{
			constexpr byte ST = 0xD9;
		}
		namespace fpatan{
			constexpr byte ST1__ST = 0xD9;
		}
		namespace fxtract{
			constexpr byte ST = 0xD9;
		}
		namespace fprem1{
			constexpr byte ST__ST1 = 0xD9;
		}
		namespace fdecstp{
			constexpr byte _ = 0xD9;
		}
		namespace fincstp{
			constexpr byte _ = 0xD9;
		}
		namespace fnstcw{
			constexpr byte m16 = 0xD9;
		}
		namespace fstcw{
			constexpr byte m16 = 0xD9;
		}
		namespace fprem{
			constexpr byte ST__ST1 = 0xD9;
		}
		namespace fyl2xp1{
			constexpr byte ST1__ST = 0xD9;
		}
		namespace fsqrt{
			constexpr byte ST = 0xD9;
		}
		namespace fsincos{
			constexpr byte ST = 0xD9;
		}
		namespace frndint{
			constexpr byte ST = 0xD9;
		}
		namespace fscale{
			constexpr byte ST__ST1 = 0xD9;
		}
		namespace fsin{
			constexpr byte ST = 0xD9;
		}
		namespace fcos{
			constexpr byte ST = 0xD9;
		}
		namespace fiadd{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fcmovb{
			constexpr byte ST__STi = 0xDA;
		}
		namespace fimul{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fcmove{
			constexpr byte ST__STi = 0xDA;
		}
		namespace ficom{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fcmovbe{
			constexpr byte ST__STi = 0xDA;
		}
		namespace ficomp{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fcmovu{
			constexpr byte ST__STi = 0xDA;
		}
		namespace fisub{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fisubr{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fucompp{
			constexpr byte ST__ST1 = 0xDA;
		}
		namespace fidiv{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fidivr{
			constexpr byte ST__m32int = 0xDA;
			constexpr byte ST__m16int = 0xDE;
		}
		namespace fild{
			constexpr byte ST__m32int = 0xDB;
			constexpr byte ST__m16int = 0xDF;
			constexpr byte ST__m64int = 0xDF;
		}
		namespace fcmovnb{
			constexpr byte ST__STi = 0xDB;
		}
		namespace fisttp{
			constexpr byte m32int__ST = 0xDB;
			constexpr byte m64int__ST = 0xDD;
			constexpr byte m16int__ST = 0xDF;
		}
		namespace fcmovne{
			constexpr byte ST__STi = 0xDB;
		}
		namespace fist{
			constexpr byte m32int__ST = 0xDB;
			constexpr byte m16int__ST = 0xDF;
		}
		namespace fcmovnbe{
			constexpr byte ST__STi = 0xDB;
		}
		namespace fistp{
			constexpr byte m32int__ST = 0xDB;
			constexpr byte m16int__ST = 0xDF;
			constexpr byte m64int__ST = 0xDF;
		}
		namespace fcmovnu{
			constexpr byte ST__STi = 0xDB;
		}
		namespace fneni {
			constexpr byte _ = 0xDB;
		}
		namespace fndisi {
			constexpr byte _ = 0xDB;
		}
		namespace fnclex{
			constexpr byte _ = 0xDB;
		}
		namespace fclex{
			constexpr byte _ = 0xDB;
		}
		namespace fninit{
			constexpr byte _ = 0xDB;
		}
		namespace finit{
			constexpr byte _ = 0xDB;
		}
		namespace fnsetpm {
			constexpr byte _ = 0xDB;
		}
		namespace fucomi{
			constexpr byte ST__STi = 0xDB;
		}
		namespace fcomi{
			constexpr byte ST__STi = 0xDB;
		}
		namespace fcom2{
			constexpr byte ST__STi = 0xDC;
		}
		namespace fcomp3{
			constexpr byte ST__STi = 0xDC;
		}
		namespace ffree{
			constexpr byte STi = 0xDD;
		}
		namespace fxch4{
			constexpr byte ST__STi = 0xDD;
		}
		namespace frstor{
			constexpr byte ST__ST1__ST2__ = 0xDD;
		}
		namespace fucom{
			constexpr byte ST__STi = 0xDD;
			constexpr byte ST__ST1 = 0xDD;
		}
		namespace fucomp{
			constexpr byte ST__STi = 0xDD;
			constexpr byte ST__ST1 = 0xDD;
		}
		namespace fnsave{
			constexpr byte m94_108__ST__ST1__ = 0xDD;
		}
		namespace fsave{
			constexpr byte m94_108__ST__ST1__ = 0xDD;
		}
		namespace fnstsw{
			constexpr byte m16 = 0xDD;
			constexpr byte AX = 0xDF;
		}
		namespace fstsw{
			constexpr byte m16 = 0xDD;
			constexpr byte AX = 0xDF;
		}
		namespace faddp{
			constexpr byte STi__ST = 0xDE;
			constexpr byte ST1__ST = 0xDE;
		}
		namespace fmulp{
			constexpr byte STi__ST = 0xDE;
			constexpr byte ST1__ST = 0xDE;
		}
		namespace fcomp5{
			constexpr byte ST__STi = 0xDE;
		}
		namespace fcompp{
			constexpr byte ST__ST1 = 0xDE;
		}
		namespace fsubrp{
			constexpr byte STi__ST = 0xDE;
			constexpr byte ST1__ST = 0xDE;
		}
		namespace fsubp{
			constexpr byte STi__ST = 0xDE;
			constexpr byte ST1__ST = 0xDE;
		}
		namespace fdivrp{
			constexpr byte STi__ST = 0xDE;
			constexpr byte ST1__ST = 0xDE;
		}
		namespace fdivp{
			constexpr byte STi__ST = 0xDE;
			constexpr byte ST1__ST = 0xDE;
		}
		namespace ffreep{
			constexpr byte STi = 0xDF;
		}
		namespace fxch7{
			constexpr byte ST__STi = 0xDF;
		}
		namespace fstp8{
			constexpr byte STi__ST = 0xDF;
		}
		namespace fstp9{
			constexpr byte STi__ST = 0xDF;
		}
		namespace fbld{
			constexpr byte ST__m80dec = 0xDF;
		}
		namespace fucomip{
			constexpr byte ST__STi = 0xDF;
		}
		namespace fbstp{
			constexpr byte m80dec__ST = 0xDF;
		}
		namespace fcomip{
			constexpr byte ST__STi = 0xDF;
		}
		namespace loopnz{
			constexpr byte rCX__rel8 = 0xE0;
		}
		namespace loopz{
			constexpr byte rCX__rel8 = 0xE1;
		}
		namespace loop{
			constexpr byte rCX__rel8 = 0xE2;
		}
		namespace jecxz{
			constexpr byte rel8__ECX = 0xE3;
		}
		namespace in{
			constexpr byte AL__imm8 = 0xE4;
			constexpr byte eAX__imm8 = 0xE5;
			constexpr byte AL__DX = 0xEC;
			constexpr byte eAX__DX = 0xED;
		}
		namespace out{
			constexpr byte imm8__AL = 0xE6;
			constexpr byte imm8__eAX = 0xE7;
			constexpr byte DX__AL = 0xEE;
			constexpr byte DX__eAX = 0xEF;
		}
		namespace call{
			constexpr byte rel16_32 = 0xE8;
			constexpr byte r_m16_32 = 0xFF;
			constexpr byte r_m64 = 0xFF;
		}
		namespace jmp{
			constexpr byte rel16_32 = 0xE9;
			constexpr byte rel8 = 0xEB;
			constexpr byte r_m16_32 = 0xFF;
			constexpr byte r_m64 = 0xFF;
		}
		namespace int1{
			constexpr byte eFlags = 0xF1;
		}
		namespace hlt{
			constexpr byte _ = 0xF4;
		}
		namespace cmc{
			constexpr byte _ = 0xF5;
		}
		namespace not{
			constexpr byte r_m8 = 0xF6;
			constexpr byte r_m16_32_64 = 0xF7;
		}
		namespace neg{
			constexpr byte r_m8 = 0xF6;
			constexpr byte r_m16_32_64 = 0xF7;
		}
		namespace mul{
			constexpr byte AX__AL__r_m8 = 0xF6;
			constexpr byte rDX__rAX__r_m16_32_64 = 0xF7;
		}
		namespace div{
			constexpr byte AL__AH__AX__r_m8 = 0xF6;
			constexpr byte rDX__rAX__r_m16_32_64 = 0xF7;
		}
		namespace idiv{
			constexpr byte AL__AH__AX__r_m8 = 0xF6;
			constexpr byte rDX__rAX__r_m16_32_64 = 0xF7;
		}
		namespace clc{
			constexpr byte _ = 0xF8;
		}
		namespace stc{
			constexpr byte _ = 0xF9;
		}
		namespace cli{
			constexpr byte _ = 0xFA;
		}
		namespace sti{
			constexpr byte _ = 0xFB;
		}
		namespace cld{
			constexpr byte _ = 0xFC;
		}
		namespace std{
			constexpr byte _ = 0xFD;
		}
		namespace inc{
			constexpr byte r_m8 = 0xFE;
			constexpr byte r_m16_32_64 = 0xFF;
		}
		namespace dec{
			constexpr byte r_m8 = 0xFE;
			constexpr byte r_m16_32_64 = 0xFF;
		}
		namespace callf{
			constexpr byte m16_32_64 = 0xFF;
		}
		namespace jmpf{
			constexpr byte m16_32_64 = 0xFF;
		}
	}
}
