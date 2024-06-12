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

#include <compiler.h>
#include <SMU.h>
#include <amd64.cgu.hxx>

#include <ELF64.hxx>

namespace testing 
{
	uint64_t memappend(void** base, void* addition, uint64_t baseSize, uint64_t additionSize)
	{
		*base = realloc(*base,baseSize+additionSize);
		memcpy(*base+baseSize,addition,additionSize);
		return baseSize+additionSize;
	}

	void main()
	{
		std::cout << ">>>>>>>>>>>> TEST BUILD RUNNING <<<<<<<<<<<<" << std::endl;
		//.
		//. required sections
		//.
		section text;
		code = &text;
		section data;
		section rodata;
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
				b69_b_imm.isSymbol = false;
				b69_b_imm.imm64 = 0x600000;
				b69_b_store.immediate = b69_b_imm;
				runtime::amd64::copy(&b69_a,&b69_b);
			}
		}
		//.
		//. generate ELF64
		//.
		{
			char strtab[] = 
				"\0"
				".text\0"
				".strtab\0"
			;
			uint64_t sh_count = 3;
			uint64_t ph_count = 0;
			uint64_t sh_index = 0;
			uint64_t ph_index = 0;
			uint64_t bufferSize = 
				 sizeof(elf64::Header)
				+(sh_count*sizeof(elf64::SectionHeader))
				+(ph_count*sizeof(elf64::ProgramHeader))
				+(text.size())
				+(sizeof(strtab))
			;
			void* fileBuffer = malloc(bufferSize);
			uint64_t contentOffset = 
				 (sh_count*sizeof(elf64::SectionHeader))
				+(ph_count*sizeof(elf64::ProgramHeader))
				+sizeof(elf64::Header)
			;
			//,
			//, generate header
			//,
			{
				elf64::Header* header = (elf64::Header*)(fileBuffer+0);
				header->Magic = elf64::Magic;
				for(uint64_t i = 0;i<7;i++)
					header->padding0[i] = 0x00;
				header->Type = 0x01;
				header->Class = 0x02;
				header->Endianess = 0x01;
				header->SystemABI = 0x00;
				header->AbiVersion = 0x00;
				header->Machine = 0x3E;
				header->ElfVersion = 0x01;
				header->Version = 0x01;
				header->EntryPoint = 0x00;
				header->SectionNameEntry = 0x02;
				header->Length = sizeof(elf64::Header);
				header->ProgramHeaderTableOffset = ph_count == 0 ? 0 : sizeof(elf64::Header)+(sh_count*sizeof(elf64::SectionHeader));
				header->ProgramHeaderCount = ph_count;
				header->ProgramHeaderSize = sizeof(elf64::ProgramHeader);
				header->SectionHeaderTableOffset = sh_count == 0 ? 0 : sizeof(elf64::Header);
				header->SectionHeaderCount = sh_count;
				header->SectionHeaderSize = sizeof(elf64::SectionHeader);
				header->Flags = 0x00;
			}
			//,
			//, generate null section
			//,
			{
				elf64::SectionHeader* header = (elf64::SectionHeader*)(
					fileBuffer
					+sizeof(elf64::Header)
					+(sh_index*sizeof(elf64::SectionHeader))
				);
				header->Type = 0x00;
				header->Flags = 0x00;
				header->Alignment = 0;
				header->OffsetInFile = 0x00;
				header->VirtualAddress = 0;
				header->SizeInFile = 0x00;
				header->EntrySize = 0;
				header->Name = 0x00;
				sh_index++;
			}
			//,
			//, generate .text section
			//,
			{
				elf64::SectionHeader* header = (elf64::SectionHeader*)(
					fileBuffer
					+sizeof(elf64::Header)
					+(sh_index*sizeof(elf64::SectionHeader))
				);
				header->Type = 0x01;
				header->Flags = elf64::shf::alloc | elf64::shf::execinstr;
				header->Alignment = 0;
				header->OffsetInFile = contentOffset;
				header->VirtualAddress = 0;
				header->SizeInFile = text.size();
				header->EntrySize = 0;
				header->Name = 0x01;
				memcpy(
					fileBuffer+contentOffset,
					(void*)text.data,
					text.size()
				);
				contentOffset+=text.size();
				sh_index++;
			}
			//,
			//, generate .strtab section
			//,
			{
				elf64::SectionHeader* header = (elf64::SectionHeader*)(
					fileBuffer
					+sizeof(elf64::Header)
					+(sh_index*sizeof(elf64::SectionHeader))
				);
				uint64_t secSize = sizeof(strtab);
				header->Type = 0x03;
				header->Flags = 0x00;
				header->Alignment = 0;
				header->OffsetInFile = contentOffset;
				header->VirtualAddress = 0;
				header->SizeInFile = secSize;
				header->EntrySize = 0;
				header->Name = 0x07;
				memcpy(
					fileBuffer+contentOffset,
					(void*)strtab,
					secSize
				);
				contentOffset+=secSize;
				sh_index++;
			}
			FILE* f = fopen("./test.elf64","w");
			fwrite(fileBuffer,bufferSize,1,f);
			fclose(f);
			//,
			//, show outputs
			//,
			{
				std::cout << ">>>>>>>>>>>> elflint <<<<<<<<<<<<" << std::endl;
				system("eu-elflint test.elf64");
				std::cout << ">>>>>>>>>>>> objdump <<<<<<<<<<<<" << std::endl;
				system("objdump -d -M amd64 -M intel -M intel-mnemonic --no-show-raw-insn --visualize-jumps=extended-color --disassembler-color=extended test.elf64");
			}
		}
	}
}