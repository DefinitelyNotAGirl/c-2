/*
 * Created Date: Thursday June 6th 2024
 * Author: Lilith
 * -----
 * Last Modified: Sat Jul 06 2024
 * Modified By: Lilith
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
#include <extint.hxx>

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
	* @brief generates a modRM byte that only specifies one reg/mem operrand
	* 
	* @param digit specified in the amd64 specification
	* @param rm reg/mem opperand
	* @return amd64 modRM byte ready for execution
	*/
	byte modRM(uint8_t digit,AddressingMode mod,Register rm)
	{
		return (digit<<3) | (mod<<6) | (rm<<0);	
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
		return register_decode_base(reg)<<3 | (mod<<6) | register_decode_base(rm)<<0;
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

	const char* register_name(Register reg) {
		switch (reg) {
			case Register::invalid: return "invalid";
			case Register::rax: return "rax";
			case Register::rcx: return "rcx";
			case Register::rdx: return "rdx";
			case Register::rbx: return "rbx";
			case Register::rsp: return "rsp";
			case Register::rbp: return "rbp";
			case Register::rsi: return "rsi";
			case Register::rdi: return "rdi";
			case Register::ah: return "ah";
			case Register::ch: return "ch";
			case Register::dh: return "dh";
			case Register::bh: return "bh";
			case Register::r8: return "r8";
			case Register::r9: return "r9";
			case Register::r10: return "r10";
			case Register::r11: return "r11";
			case Register::r12: return "r12";
			case Register::r13: return "r13";
			case Register::r14: return "r14";
			case Register::r15: return "r15";
			case Register::mmx0: return "mmx0";
			case Register::mmx1: return "mmx1";
			case Register::mmx2: return "mmx2";
			case Register::mmx3: return "mmx3";
			case Register::mmx4: return "mmx4";
			case Register::mmx5: return "mmx5";
			case Register::mmx6: return "mmx6";
			case Register::mmx7: return "mmx7";
			case Register::xmm0: return "xmm0";
			case Register::xmm1: return "xmm1";
			case Register::xmm2: return "xmm2";
			case Register::xmm3: return "xmm3";
			case Register::xmm4: return "xmm4";
			case Register::xmm5: return "xmm5";
			case Register::xmm6: return "xmm6";
			case Register::xmm7: return "xmm7";
			case Register::ymm0: return "ymm0";
			case Register::ymm1: return "ymm1";
			case Register::ymm2: return "ymm2";
			case Register::ymm3: return "ymm3";
			case Register::ymm4: return "ymm4";
			case Register::ymm5: return "ymm5";
			case Register::ymm6: return "ymm6";
			case Register::ymm7: return "ymm7";
			case Register::cr0: return "cr0";
			case Register::cr2: return "cr2";
			case Register::cr3: return "cr3";
			case Register::cr4: return "cr4";
			case Register::cr8: return "cr8";
			case Register::gdtr: return "gdtr";
			case Register::idtr: return "idtr";
			case Register::ldtr: return "ldtr";
			case Register::efer: return "efer";
			case Register::star: return "star";
			case Register::lstar: return "lstar";
			case Register::cstar: return "cstar";
			case Register::sfmask: return "sfmask";
			case Register::fs_base: return "fs_base";
			case Register::gs_base: return "gs_base";
			case Register::kernel_gs_base: return "kernel_gs_base";
			case Register::tsc_aux: return "tsc_aux";
			case Register::syscfg: return "syscfg";
			case Register::iorr_base0: return "iorr_base0";
			case Register::iorrmask0: return "iorrmask0";
			case Register::iorr_base1: return "iorr_base1";
			case Register::iorrmask1: return "iorrmask1";
			case Register::ls_cfg: return "ls_cfg";
			case Register::ic_cfg: return "ic_cfg";
			case Register::dc_cfg: return "dc_cfg";
			case Register::bu_cfg: return "bu_cfg";
			case Register::mc0_ctl: return "mc0_ctl";
			case Register::mc0_status: return "mc0_status";
			case Register::mc0_addr: return "mc0_addr";
			case Register::mc0_misc: return "mc0_misc";
			case Register::perf_ctl0: return "perf_ctl0";
			case Register::perf_ctr0: return "perf_ctr0";
			case Register::perf_ctl1: return "perf_ctl1";
			case Register::perf_ctr1: return "perf_ctr1";
			case Register::top_mem: return "top_mem";
			case Register::top_mem2: return "top_mem2";
			case Register::vm_cr: return "vm_cr";
			case Register::vm_hsave_pa: return "vm_hsave_pa";
			default: return "invalid amd64 register";
		}
	}

	Register string_to_register(const std::string& reg_str) {
		if (reg_str == "invalid") return Register::invalid;
		if (reg_str == "rax") return Register::rax;
		if (reg_str == "rcx") return Register::rcx;
		if (reg_str == "rdx") return Register::rdx;
		if (reg_str == "rbx") return Register::rbx;
		if (reg_str == "rsp") return Register::rsp;
		if (reg_str == "rbp") return Register::rbp;
		if (reg_str == "rsi") return Register::rsi;
		if (reg_str == "rdi") return Register::rdi;
		if (reg_str == "ah") return Register::ah;
		if (reg_str == "ch") return Register::ch;
		if (reg_str == "dh") return Register::dh;
		if (reg_str == "bh") return Register::bh;
		if (reg_str == "r8") return Register::r8;
		if (reg_str == "r9") return Register::r9;
		if (reg_str == "r10") return Register::r10;
		if (reg_str == "r11") return Register::r11;
		if (reg_str == "r12") return Register::r12;
		if (reg_str == "r13") return Register::r13;
		if (reg_str == "r14") return Register::r14;
		if (reg_str == "r15") return Register::r15;
		if (reg_str == "mmx0") return Register::mmx0;
		if (reg_str == "mmx1") return Register::mmx1;
		if (reg_str == "mmx2") return Register::mmx2;
		if (reg_str == "mmx3") return Register::mmx3;
		if (reg_str == "mmx4") return Register::mmx4;
		if (reg_str == "mmx5") return Register::mmx5;
		if (reg_str == "mmx6") return Register::mmx6;
		if (reg_str == "mmx7") return Register::mmx7;
		if (reg_str == "xmm0") return Register::xmm0;
		if (reg_str == "xmm1") return Register::xmm1;
		if (reg_str == "xmm2") return Register::xmm2;
		if (reg_str == "xmm3") return Register::xmm3;
		if (reg_str == "xmm4") return Register::xmm4;
		if (reg_str == "xmm5") return Register::xmm5;
		if (reg_str == "xmm6") return Register::xmm6;
		if (reg_str == "xmm7") return Register::xmm7;
		if (reg_str == "ymm0") return Register::ymm0;
		if (reg_str == "ymm1") return Register::ymm1;
		if (reg_str == "ymm2") return Register::ymm2;
		if (reg_str == "ymm3") return Register::ymm3;
		if (reg_str == "ymm4") return Register::ymm4;
		if (reg_str == "ymm5") return Register::ymm5;
		if (reg_str == "ymm6") return Register::ymm6;
		if (reg_str == "ymm7") return Register::ymm7;
		if (reg_str == "cr0") return Register::cr0;
		if (reg_str == "cr2") return Register::cr2;
		if (reg_str == "cr3") return Register::cr3;
		if (reg_str == "cr4") return Register::cr4;
		if (reg_str == "cr8") return Register::cr8;
		if (reg_str == "gdtr") return Register::gdtr;
		if (reg_str == "idtr") return Register::idtr;
		if (reg_str == "ldtr") return Register::ldtr;
		if (reg_str == "efer") return Register::efer;
		if (reg_str == "star") return Register::star;
		if (reg_str == "lstar") return Register::lstar;
		if (reg_str == "cstar") return Register::cstar;
		if (reg_str == "sfmask") return Register::sfmask;
		if (reg_str == "fs_base") return Register::fs_base;
		if (reg_str == "gs_base") return Register::gs_base;
		if (reg_str == "kernel_gs_base") return Register::kernel_gs_base;
		if (reg_str == "tsc_aux") return Register::tsc_aux;
		if (reg_str == "syscfg") return Register::syscfg;
		if (reg_str == "iorr_base0") return Register::iorr_base0;
		if (reg_str == "iorrmask0") return Register::iorrmask0;
		if (reg_str == "iorr_base1") return Register::iorr_base1;
		if (reg_str == "iorrmask1") return Register::iorrmask1;
		if (reg_str == "ls_cfg") return Register::ls_cfg;
		if (reg_str == "ic_cfg") return Register::ic_cfg;
		if (reg_str == "dc_cfg") return Register::dc_cfg;
		if (reg_str == "bu_cfg") return Register::bu_cfg;
		if (reg_str == "mc0_ctl") return Register::mc0_ctl;
		if (reg_str == "mc0_status") return Register::mc0_status;
		if (reg_str == "mc0_addr") return Register::mc0_addr;
		if (reg_str == "mc0_misc") return Register::mc0_misc;
		if (reg_str == "perf_ctl0") return Register::perf_ctl0;
		if (reg_str == "perf_ctr0") return Register::perf_ctr0;
		if (reg_str == "perf_ctl1") return Register::perf_ctl1;
		if (reg_str == "perf_ctr1") return Register::perf_ctr1;
		if (reg_str == "top_mem") return Register::top_mem;
		if (reg_str == "top_mem2") return Register::top_mem2;
		if (reg_str == "vm_cr") return Register::vm_cr;
		if (reg_str == "vm_hsave_pa") return Register::vm_hsave_pa;
		return Register::invalid;
	}
}

namespace runtime::amd64
{
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wnarrowing"
	void copy(variable* src, variable* dst)
	{
		#if false
			code->placeSymbol(SymbolType::CodeLocation,0,".debug: copy, "+src->name+" ==> "+dst->name);
		#endif
		if(src->storageArch != Architecture::AMD64)
			compilerBug("invalid source architecture: "+std::to_string((uint64_t)src->storageArch),originCoreHere,source(),"");
		if(dst->storageArch != Architecture::AMD64)
			compilerBug("invalid destination architecture: "+std::to_string((uint64_t)dst->storageArch),originCoreHere,source(),"");
		::amd64::VariableStorage* srcStore = (::amd64::VariableStorage*)src->storage;
		::amd64::VariableStorage* dstStore = (::amd64::VariableStorage*)dst->storage;
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
			(
				(srcStore->mode == ::amd64::StorageMode::DirectRegister)
				&&
				(dstStore->mode == ::amd64::StorageMode::IndirectRegister)
			)
		)
		{
			if(dstStore->displacement.imm64 == 0x00)
			{
				code->push({
					::amd64::prefix::REX(1,::amd64::register_decode_rex(srcStore->reg),0,::amd64::register_decode_rex(dstStore->reg)),
					::amd64::opcode::mov::rm16_32_64__r16_32_64,
					::amd64::modRM(srcStore->reg,::amd64::AddressingMode::RegisterIndirect,dstStore->reg)
				});
			}
			else if(positive(dstStore->displacement.imm64) <= 0xFF)
			{
				code->push({
					::amd64::prefix::REX(1,::amd64::register_decode_rex(srcStore->reg),0,::amd64::register_decode_rex(dstStore->reg)),
					::amd64::opcode::mov::rm16_32_64__r16_32_64,
					::amd64::modRM(srcStore->reg,::amd64::AddressingMode::RegisterIndirect_disp8,dstStore->reg),
					(byte)copySignBit<uint64_t,uint8_t>(dstStore->displacement.imm64)
				});
				if(srcStore->displacement.isSymbol)
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-1,
							1,
							smu::RelocationType::Absolute,
							dstStore->displacement.symbol
						)
					);
				}
			}
			else if(positive(dstStore->displacement.imm64) <= 0xFFFFFFFF)
			{
				code->push({
					::amd64::prefix::REX(1,::amd64::register_decode_rex(srcStore->reg),0,::amd64::register_decode_rex(dstStore->reg)),
					::amd64::opcode::mov::rm16_32_64__r16_32_64,
					::amd64::modRM(srcStore->reg,::amd64::AddressingMode::RegisterIndirect_disp32,dstStore->reg)
				});
				code->push(::amd64::imm32(copySignBit<uint64_t,uint32_t>(dstStore->displacement.imm64)));
				if(srcStore->displacement.isSymbol)
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							dstStore->displacement.symbol
						)
					);
				}
			}
			else
			{
				compilerBug("diplacement exceeds 32 bits.");
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
							::amd64::prefix::REX((opsize == 8),0,0,::amd64::register_decode_rex(srcStore->reg)),
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
							::amd64::prefix::REX((opsize == 8),0,0,::amd64::register_decode_rex(dstStore->reg)),
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
	void clear(variable* target);
	void thread(std::string& symbol);
	void call(function* func);
	void enter(uint64_t frameSize);
	void leave();

	variable* UnsignedIntegerAddition(variable* a, variable* b)
	{
		if(a->storageArch != Architecture::AMD64)
			compilerBug("invalid source architecture: "+std::to_string((uint64_t)a->storageArch),originCoreHere,source(),"");
		if(b->storageArch != Architecture::AMD64)
			compilerBug("invalid destination architecture: "+std::to_string((uint64_t)b->storageArch),originCoreHere,source(),"");
		variable* dst = new variable(*a);
		dst->storage = (void*)(new ::amd64::VariableStorage);
		::amd64::VariableStorage* dstStore = (::amd64::VariableStorage*)dst->storage;
		dstStore->mode = ::amd64::StorageMode::DirectRegister;
		dstStore->reg = ::amd64::Register::rax;
		runtime::amd64::copy(a,dst);
		::amd64::VariableStorage* bStore = (::amd64::VariableStorage*)b->storage;
		if(bStore->mode == ::amd64::StorageMode::DirectRegister)
		{
			code->push({
				::amd64::prefix::REX(1,::amd64::register_decode_rex(dstStore->reg),0,::amd64::register_decode_rex(bStore->reg)),
				::amd64::opcode::add::r16_32_64__rm16_32_64,
				::amd64::modRM(dstStore->reg,bStore->reg)
			});
		}
		else if(bStore->mode == ::amd64::StorageMode::DirectImmediate)
		{
			code->push({
				::amd64::prefix::REX(1,0,0,0),
				::amd64::opcode::add::rAX__imm16_32
			});
			code->push(::amd64::imm32(bStore->immediate.imm64));
			if(bStore->immediate.isSymbol)
			{
				compilerBug("unimplemented: immediate symbol addition");
			}
		}
		else compilerBug("unimplemented uint addition storage: "+std::to_string((uint64_t)bStore->mode));
		return dst;
	}

	void RelativeControlTransfer(ImmediateValue offset)
	{
		code->push({
			::amd64::opcode::jmp::rel16_32,
			::amd64::imm32(offset.imm64)
		});
		if(offset.isSymbol) {
			code->Relocations.push_back(
				smu::RelocationEntry(
					code->size()-4,
					4,
					smu::RelocationType::Relative,
					offset.symbol
				)
			);
		}
	}

	void AbsoluteControlTransfer(ImmediateValue address)
	{
		code->push({
			::amd64::prefix::REX(1,0,0,0),
			::amd64::opcode::mov::r16_32_64__imm16_32_64 + ::amd64::register_decode_base(::amd64::Register::rax),
			::amd64::imm64(address.imm64)
		});
		if(address.isSymbol) {
			code->Relocations.push_back(
				smu::RelocationEntry(
					code->size()-8,
					8,
					smu::RelocationType::Relative,
					address.symbol
				)
			);
		}
		code->push({
			::amd64::prefix::REX(1,0,0,0),
			::amd64::opcode::jmp::rm16_32_64,
			::amd64::modRM(4,::amd64::AddressingMode::RegisterDirect,::amd64::Register::rax)
		});
	}

	static void util_save(uint32_t offs, ::amd64::Register reg)
	{
		code->push({
			::amd64::prefix::REX(1,::amd64::register_decode_rex(reg),0,0),
			::amd64::opcode::mov::rm16_32_64__r16_32_64,
			::amd64::modRM(reg,::amd64::AddressingMode::RegisterIndirect_disp32,::amd64::Register::rbp)
		});
		code->push(::amd64::imm32(offs));
	}
	static void util_load(uint32_t offs, ::amd64::Register reg)
	{
		code->push({
			::amd64::prefix::REX(1,::amd64::register_decode_rex(reg),0,0),
			::amd64::opcode::mov::r16_32_64__rm16_32_64,
			::amd64::modRM(reg,::amd64::AddressingMode::RegisterIndirect_disp32,::amd64::Register::rbp)
		});
		code->push(::amd64::imm32(offs));
	}
	/**
	 * @brief save all user registers to the stack except the stack pointer, the frame pointer and the thread-data pointer
	 * @return stack frame offset of saved data
	 */
	uint64_t SaveAll()
	{
		#if false
			code->placeSymbol(SymbolType::CodeLocation,0,".debug: saveall");
		#endif
		stackframe* stack = currentScope->func->stack;
		uint32_t base = stack->push(104);
		util_save(base+0x00,::amd64::Register::rax);
		util_save(base+0x08,::amd64::Register::rbx);
		util_save(base+0x10,::amd64::Register::rcx);
		util_save(base+0x18,::amd64::Register::rdx);
		util_save(base+0x20,::amd64::Register::rdi);
		util_save(base+0x28,::amd64::Register::rsi);
		util_save(base+0x30,::amd64::Register::r8 );
		util_save(base+0x38,::amd64::Register::r9 );
		util_save(base+0x40,::amd64::Register::r10);
		util_save(base+0x48,::amd64::Register::r11);
		util_save(base+0x50,::amd64::Register::r12);
		util_save(base+0x58,::amd64::Register::r13);
		util_save(base+0x60,::amd64::Register::r14);
		return base;
	}
	/**
	 * @brief load all user registers from the stack except the stack pointer, the frame pointer and the thread-data pointer
	 */
	void LoadAll(uint64_t offset)
	{
		#if false
			code->placeSymbol(SymbolType::CodeLocation,0,".debug: loadall");
		#endif
		stackframe* stack = currentScope->func->stack;
		uint32_t base = offset;
		util_load(base+0x00,::amd64::Register::rax);
		util_load(base+0x08,::amd64::Register::rbx);
		util_load(base+0x10,::amd64::Register::rcx);
		util_load(base+0x18,::amd64::Register::rdx);
		util_load(base+0x20,::amd64::Register::rdi);
		util_load(base+0x28,::amd64::Register::rsi);
		util_load(base+0x30,::amd64::Register::r8 );
		util_load(base+0x38,::amd64::Register::r9 );
		util_load(base+0x40,::amd64::Register::r10);
		util_load(base+0x48,::amd64::Register::r11);
		util_load(base+0x50,::amd64::Register::r12);
		util_load(base+0x58,::amd64::Register::r13);
		util_load(base+0x60,::amd64::Register::r14);
		return base;
	}

	/**
	 * @brief call function
	 */
	void call(function* func)
	{
		code->push({
			::amd64::opcode::call::rel16_32,
			0,0,0,0
		});
		code->Relocations.push_back(
			smu::RelocationEntry(
				code->size()-4,
				4,
				smu::RelocationType::Relative,
				func->symbol
			)
		);
	}
	#pragma GCC diagnostic pop
}
