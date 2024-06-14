/*
 * Created Date: Thursday June 6th 2024
 * Author: Lilith
 * -----
 * Last Modified: Thursday June 6th 2024 10:09:55 am
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

#include <issues.hxx>
#include <SMU.h>
#include <amd64.cgu.hxx>

using namespace issues;

namespace amd64
{
	uint64_t imm64(uint64_t value)
	{
		return value;
	}

	uint32_t imm32(uint32_t value)
	{
		return value;
	}

	uint16_t imm16(uint16_t value)
	{
		return value;
	}

	/**
	 * @brief this is short hand for modRM(reg,AddressingMode::RegisterDirect,rm)
	 * 
	 * @param reg reg operrand
	 * @param rm reg/mem opperand
	 * @return amd64 modRM byte ready for execution
	 *
	 */
	byte modRM(Register reg,Register rm)
	{
		return modRM(reg,AddressingMode::RegisterDirect,rm);
	}
	/**
	 * @brief
	 * 
	 * @param reg reg operrand
	 * @param rm reg/mem opperand
	 * @return amd64 modRM byte ready for execution
	 */
	byte modRM(Register reg,AddressingMode mod,Register rm)
	{
		return (reg<<3) | (mod<<6) | (rm<<0);
	}
	/**
	 * @brief
	 * 
	 * @param scale must be either 1,2,4 or 8 
	 * @param index register holding the index
	 * @param base register holding the base address
	 * @return amd64 SIB byte ready for execution
	 */
	byte SIB(uint8_t scale, Register index, Register base)
	{
		return (scale<<6) | (index<<3) | (base<<0);
	}

	namespace prefix
	{
		byte REX(bool W, bool R, bool X, bool B)
		{
			return 0x40 | (W<<3) | (R<<2) | (X<<1) | (B<<0);
		}
		namespace legacy
		{
		}
	}

	namespace opcode {
	}
}

namespace runtime::amd64
{
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wnarrowing"
	void copy(variable* src, variable* dst)
	{
		if(src->storageArch != Architecture::AMD64)
			compilerBug("invalid architecture",originCoreHere,source(),"");
		if(dst->storageArch != Architecture::AMD64)
			compilerBug("invalid architecture",originCoreHere,source(),"");
		::amd64::VariableStorage* srcStore = src->storage;
		::amd64::VariableStorage* dstStore = dst->storage;
//,####################################################################################################################
//,####################################################################################################################
//, ██████  ██████                       ██       ██████  ██████
//, ██   ██ ██   ██                       ██      ██   ██ ██   ██
//, ██   ██ ██████      █████ █████ █████  ██     ██   ██ ██████
//, ██   ██ ██   ██                       ██      ██   ██ ██   ██
//, ██████  ██   ██                      ██       ██████  ██   ██
//,####################################################################################################################
//,####################################################################################################################
		if( 
			(srcStore->mode == ::amd64::StorageMode::DirectRegister)
			&&
			(dstStore->mode == ::amd64::StorageMode::DirectRegister)
		)
		{
			code->push({
				::amd64::prefix::REX(0,((srcStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
				::amd64::opcode::mov::r16_32_64__rm16_32_64,
				::amd64::modRM(srcStore->reg,dstStore->reg)
			});
		}
//,####################################################################################################################
//,####################################################################################################################
//, ██████  ██████                       ██       ██ ██████
//, ██   ██ ██   ██                       ██      ██ ██   ██
//, ██   ██ ██████      █████ █████ █████  ██     ██ ██████
//, ██   ██ ██   ██                       ██      ██ ██   ██
//, ██████  ██   ██                      ██       ██ ██   ██
//,####################################################################################################################
//,####################################################################################################################
		if(
			(srcStore->mode == ::amd64::StorageMode::DirectRegister)
			&&
			(dstStore->mode == ::amd64::StorageMode::IndirectRegister)
		)
		{
			if(dstStore->displacement.imm64 == 0x00)
			{
				code->push({
					::amd64::prefix::REX(1,((srcStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
					::amd64::opcode::mov::r16_32_64__rm16_32_64,
					::amd64::modRM(srcStore->reg,::amd64::AddressingMode::RegisterIndirect,dstStore->reg)
				});
			}
			else if(dstStore->displacement.imm64 <= 0xFF)
			{
				code->push({
					::amd64::prefix::REX(1,((srcStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
					::amd64::opcode::mov::r16_32_64__rm16_32_64,
					::amd64::modRM(srcStore->reg,::amd64::AddressingMode::RegisterIndirect_disp8,dstStore->reg),
					(byte)srcStore->displacement.imm64
				});
				if(srcStore->displacement.isSymbol)
					compilerBug("linker information not implemented, cant use symbol",originCoreHere,source(),"");
			}
			else if(dstStore->displacement.imm64 <= 0xFFFFFFFF)
			{
				code->push({
					::amd64::prefix::REX(1,((srcStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
					::amd64::opcode::mov::r16_32_64__rm16_32_64,
					::amd64::modRM(srcStore->reg,::amd64::AddressingMode::RegisterIndirect_disp32,dstStore->reg)
				});
				code->push(::amd64::imm32(dstStore->displacement.imm64));
				if(srcStore->displacement.isSymbol)
					compilerBug("linker information not implemented, cant use symbol",originCoreHere,source(),"");
			}
		}
//,####################################################################################################################
//,####################################################################################################################
//, ██ ██████                       ██       ██ ██████
//, ██ ██   ██                       ██      ██ ██   ██
//, ██ ██████      █████ █████ █████  ██     ██ ██████
//, ██ ██   ██                       ██      ██ ██   ██
//, ██ ██   ██                      ██       ██ ██   ██
//,####################################################################################################################
//,####################################################################################################################
		if(
			(srcStore->mode == ::amd64::StorageMode::IndirectRegister)
			&&
			(dstStore->mode == ::amd64::StorageMode::IndirectRegister)
		)
		{
			uint64_t bytes = src->dataType->size <= dst->dataType->size ? src->dataType->size : dst->dataType->size;
			uint64_t disp = 0;
			uint64_t mbdisp = srcStore->displacement.imm64 >= dstStore->displacement.imm64 ? srcStore->displacement.imm64 : dstStore->displacement.imm64;
			if(srcStore->displacement.isSymbol || dstStore->displacement.isSymbol)
				compilerBug("unimplemented");
			::amd64::AddressingMode addrMode = (bytes+mbdisp) <= (0b01111111) ? ::amd64::AddressingMode::RegisterIndirect_disp8 : ::amd64::AddressingMode::RegisterIndirect_disp32;
			uint64_t opsize = 8;
			while(opsize > 0)
			{
				while(bytes >= opsize)
				{
					//,
					//, copy data to rax
					//,
					{
						if(opsize == 2)code->push({::amd64::prefix::legacy::OperandSizeOverride});
						code->push({
							::amd64::prefix::REX((opsize == 8),((srcStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
							(opsize > 1 ? ::amd64::opcode::mov::r16_32_64__rm16_32_64 : ::amd64::opcode::mov::r8__rm8),
							::amd64::modRM(::amd64::Register::rax,addrMode,srcStore->reg)
						});
						//+
						//+	base == rsp
						//+
						if(srcStore->reg == ::amd64::Register::rsp || srcStore->reg == ::amd64::Register::r12)
						{
							code->push({
								::amd64::SIB(0,::amd64::Register::rsp,::amd64::Register::rsp)
							});
						}
						switch(addrMode)
						{
							case(::amd64::AddressingMode::RegisterIndirect_disp8):
								code->push({(byte)disp+srcStore->displacement.imm64});
								break;
							case(::amd64::AddressingMode::RegisterIndirect_disp32):
								code->push(::amd64::imm32(disp+srcStore->displacement.imm64));
								break;
							default:
								compilerBug("this code is supposed to be unreachable.");
						}	
					}
					//,
					//, copy data from rax to dst
					//,
					{
						if(opsize == 2)code->push({::amd64::prefix::legacy::OperandSizeOverride});
						code->push({
							::amd64::prefix::REX((opsize == 8),((dstStore->reg & (1<<4))>>4),0,((dstStore->reg & (1<<4))>>4)),
							(opsize > 1 ? ::amd64::opcode::mov::rm16_32_64__r16_32_64 : ::amd64::opcode::mov::rm8__r8),
							::amd64::modRM(::amd64::Register::rax,addrMode,dstStore->reg)
						});
						//+
						//+	base == rsp
						//+
						if(dstStore->reg == ::amd64::Register::rsp || dstStore->reg == ::amd64::Register::r12)
						{
							code->push({
								::amd64::SIB(0,::amd64::Register::rsp,::amd64::Register::rsp)
							});
						}
						switch(addrMode)
						{
							case(::amd64::AddressingMode::RegisterIndirect_disp8):
								code->push({(byte)disp+dstStore->displacement.imm64});
								break;
							case(::amd64::AddressingMode::RegisterIndirect_disp32):
								code->push(::amd64::imm32(disp+dstStore->displacement.imm64));
								break;
							default:
								compilerBug("this code is supposed to be unreachable.");
						}
					}
					disp+=opsize;
					bytes-=opsize;
				}
				opsize/=2;
			}
		}
//,####################################################################################################################
//,####################################################################################################################
//, ██ ██████                       ██       ██ ██████  ███    ███
//, ██ ██   ██                       ██      ██ ██   ██ ████  ████
//, ██ ██████      █████ █████ █████  ██     ██ ██   ██ ██ ████ ██
//, ██ ██   ██                       ██      ██ ██   ██ ██  ██  ██
//, ██ ██   ██                      ██       ██ ██████  ██      ██
//,####################################################################################################################
//,####################################################################################################################
		if(
			(srcStore->mode == ::amd64::StorageMode::IndirectRegister)
			&&
			(dstStore->mode == ::amd64::StorageMode::IndirectImmediate)
		)
		{
			//,
			//, load base address to rcx
			//,
			::amd64::Register Base = ::amd64::Register::rcx;
			{
				code->push({
					::amd64::prefix::REX(1,0,0,0),
					::amd64::opcode::mov::r16_32_64__imm16_32_64 + ((byte)Base)
				});
				code->push(::amd64::imm64(dstStore->immediate.imm64));
				if(dstStore->immediate.isSymbol){
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-8,
							8,
							smu::RelocationType::Absolute,
							dstStore->immediate.symbol
						)
					);
				}
			}
			//,
			//, create copy of dst variable
			//,
			variable __dst = *dst;
			{
				::amd64::VariableStorage __dst_store = *dstStore;
				__dst_store.mode = ::amd64::StorageMode::IndirectRegister;
				__dst_store.reg = Base;
				__dst.storage = &__dst_store;
			}
			copy(src,&__dst);
		}
	}
	#pragma GCC diagnostic pop
	void clear(variable* target);
	void thread(std::string& symbol);
	void call(function* func);
	void enter(uint64_t frameSize);
	void leave();
}
