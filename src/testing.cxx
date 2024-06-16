/*
 * Created Date: Saturday June 8th 2024
 * Author: Lilith
 * -----
 * Last Modified: Saturday June 8th 2024 11:54:00 pm
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

#include <dump.hxx>
#include <compiler.h>
#include <SMU.h>
#include <cgu.h>
#include <amd64.cgu.hxx>
#include <ELF64.hxx>
#include <output.hxx>
#include <issues.hxx>
using namespace issues;

void test_main()
{
	code = &text;
	std::cout << ">>>>>>>>>>>> TEST BUILD RUNNING <<<<<<<<<<<<" << std::endl;
	//.
	//. data types
	//.
	type u64;
	u64.size = 8;
	u64.mangledName = "u64";
	u64.name = "u64";
	type b69;
	b69.size = 0x45;
	b69.mangledName = "b69";
	b69.name = "b69";
	//.
	//. symbols
	//.
	{
		//+
		//+ _test1
		//+
		{
			SymbolMap.insert(std::pair<std::string,uint64_t>(
				"__test1",
				elf64::symtab.size()/sizeof(elf64::SymbolTableEntry)
			));
			elf64::symtab.push(
				elf64::SymbolTableEntry(
					elf64::strtab.size(),
					0x00,
					0x00,
					(uint16_t)elf64::SectionTableIndex::ABS,
					0x0123456789ABCDEF,
					0
				)
			);
			elf64::strtab.push(
				"__test1",
				sizeof("__test1")+1
			);
		}
	}
	//.
	//. runtime code
	//.
	{
		//+
		//+ IR --> IR
		//+
		if(false){
			//- b69_a
			variable b69_a;
			b69_a.dataType = &b69;
			b69_a.storageArch = Architecture::AMD64;
			amd64::VariableStorage b69_a_store;
			b69_a.storage = &b69_a_store;
			b69_a_store.mode = amd64::StorageMode::IndirectRegister;
			ImmediateValue b69_a_disp;
			b69_a_disp.isSymbol = false;
			b69_a_disp.imm64 = 0x40;
			b69_a_store.displacement = b69_a_disp;
			b69_a_store.reg = amd64::Register::rsp;
			//- b69_b
			variable b69_b;
			b69_b.dataType = &b69;
			b69_b.storageArch = Architecture::AMD64;
			amd64::VariableStorage b69_b_store;
			b69_b.storage = &b69_b_store;
			b69_b_store.mode = amd64::StorageMode::IndirectRegister;
			ImmediateValue b69_b_disp;
			b69_b_disp.isSymbol = false;
			b69_b_disp.imm64 = 0x90;
			b69_b_store.displacement = b69_b_disp;
			b69_b_store.reg = amd64::Register::r14;
			runtime::amd64::copy(&b69_a,&b69_b);
		}
		//+
		//+ IR --> IDM
		//+
		{
			//- b69_a
			variable b69_a;
			b69_a.dataType = &b69;
			b69_a.storageArch = Architecture::AMD64;
			amd64::VariableStorage b69_a_store;
			b69_a.storage = &b69_a_store;
			b69_a_store.mode = amd64::StorageMode::IndirectRegister;
			ImmediateValue b69_a_disp;
			b69_a_disp.isSymbol = false;
			b69_a_disp.imm64 = 0x40;
			b69_a_store.displacement = b69_a_disp;
			b69_a_store.reg = amd64::Register::rsp;
			//- b69_b
			variable b69_b;
			b69_b.dataType = &b69;
			b69_b.storageArch = Architecture::AMD64;
			amd64::VariableStorage b69_b_store;
			b69_b.storage = &b69_b_store;
			b69_b_store.mode = amd64::StorageMode::IndirectImmediate;
			ImmediateValue b69_b_disp;
			b69_b_disp.isSymbol = false;
			b69_b_disp.imm64 = 0x00;
			b69_b_store.displacement = b69_b_disp;
			ImmediateValue b69_b_imm;
			b69_b_imm.isSymbol = true;
			b69_b_imm.imm64 = 0x600000;
			b69_b_imm.symbol = "__test1";
			b69_b_store.immediate = b69_b_imm;
			runtime::amd64::copy(&b69_a,&b69_b);
		}
	}
	output::generateElf64();
}
