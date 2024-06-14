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
#include <issues.hxx>
using namespace issues;

namespace testing 
{
	#define strtab_initial "\0.strtab"
	char* initStrtabData()
	{
		char* data = strtab_initial;
		char* daData = (char*)malloc(sizeof(strtab_initial));
		memcpy(daData,data,sizeof(strtab_initial));
		return daData;
	}
	uint64_t strtabPos = sizeof(strtab_initial);
	char* strtabData = initStrtabData();
	uint64_t sh_count = 2;
	uint64_t ph_count = 0;
	uint64_t bufferSize = sizeof(elf64::Header)+(sh_count*sizeof(elf64::SectionHeader))+sizeof(strtab_initial);
	struct eos {
		uint64_t bytes;
		void* data;
		elf64::SectionType type;
		uint64_t Flags;
		uint64_t name;
		eos(uint64_t bytes,void* data,elf64::SectionType type,uint64_t Flags,uint64_t name)
			:bytes(bytes),data(data),type(type),Flags(Flags),name(name){}
	};
	std::vector<eos> outputSections;
	void addSection(std::string name, section* s,uint64_t Flags,elf64::SectionType type);

	std::map<std::string,uint64_t> SymbolMap;

	void GenerateSymTabSection()
	{
		section* symtab = new section;
		elf64::SymbolTableEntry se;
		//. generate null entry
		memset((void*)&se,0x00,sizeof(se));
		symtab->push(se);
		//. generate symbol entires
		uint64_t i = 1;
		for(cgu::Symbol& sym : cgu::symbols)
		{
			SymbolMap.insert({std::pair<std::string,uint64_t>(sym.text,i++)});
			bufferSize+=sym.text.length()+1;
			strtabData = (char*)realloc((void*)strtabData,strtabPos+sym.text.length()+1);
			memcpy(strtabData+strtabPos,sym.text.data(),sym.text.length()+1);
			uint64_t namePos = strtabPos;
			strtabPos+=sym.text.length()+1;
			se.name = namePos;
			se.SectionTableIndex = sym.SectionIndex;
			se.SymbolValue = sym.offset;
			se.SymbolSize = sym.size;
			symtab->push(se);
		}
		addSection(".symtab",symtab,0x00,elf64::SectionType::SHT_SYMTAB);
	}

	void GenerateRelaSection(std::string name,section* s)
	{
		section* rs = new section;
		for(smu::RelocationEntry& re : s->Relocations)
		{
			auto it = SymbolMap.find(re.symbol);
			if(it == SymbolMap.end())
				compilerBug("symbol not found");
			uint64_t symNum = SymbolMap.extract(it).mapped();
			elf64::RelocationType rtype;
			if(re.type == smu::RelocationType::Absolute && re.size == 8)
				rtype = elf64::RelocationType::R_X86_64_64;
			else
				compilerBug("unimplemented");
			rs->push(elf64::RelocationEntry(re.offset,symNum,rtype,0));
		}
		addSection(name,rs,0x00,elf64::SectionType::SHT_RELA);
	}

	void addSection(std::string name, section* s,uint64_t Flags,elf64::SectionType type)
	{
		bufferSize+=sizeof(elf64::SectionHeader);
		bufferSize+=s->size();
		sh_count++;
		uint64_t namePos = 0;
		if(name != "")
		{
			bufferSize+=name.length()+1;
			strtabData = (char*)realloc((void*)strtabData,strtabPos+name.length()+1);
			memcpy(strtabData+strtabPos,name.data(),name.length()+1);
			namePos = strtabPos;
			strtabPos+=name.length()+1;
		}
		outputSections.push_back(eos(s->size(),s->data,type,Flags,namePos));
		if(s->Relocations.size() > 0)
			GenerateRelaSection(".rela"+name,s);
	}

	#define contentBuffer (fileBuffer+contentOffset)
	#define headerBuffer (fileBuffer+headerOffset)

	void finalize()
	{
		std::cout << "finalizing..." << std::endl;
		void* fileBuffer = malloc(bufferSize);
		uint64_t contentOffset = (
			 (sh_count*sizeof(elf64::SectionHeader))
			+(ph_count*sizeof(elf64::ProgramHeader))
			+sizeof(elf64::Header)
		);
		uint64_t headerOffset = 0;
		//,
		//, generate header
		//,
		{
			std::cout << "generating header..." << std::endl;
			elf64::Header* header = (elf64::Header*)headerBuffer;
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
			header->SectionNameEntry = 0x01;
			header->Length = sizeof(elf64::Header);
			header->ProgramHeaderTableOffset = ph_count == 0 ? 0 : sizeof(elf64::Header)+(sh_count*sizeof(elf64::SectionHeader));
			header->ProgramHeaderCount = ph_count;
			header->ProgramHeaderSize = sizeof(elf64::ProgramHeader);
			header->SectionHeaderTableOffset = sh_count == 0 ? 0 : sizeof(elf64::Header);
			header->SectionHeaderCount = sh_count;
			header->SectionHeaderSize = sizeof(elf64::SectionHeader);
			header->Flags = 0x00;

			headerOffset+=sizeof(elf64::Header);
		}
		//,
		//, generate null entry
		//,
		{
			std::cout << "generating null entry..." << std::endl;
			memset(headerBuffer,0x00,sizeof(elf64::SectionHeader));
			headerOffset+=sizeof(elf64::SectionHeader);
		}
		//,
		//, generate strtab section
		//,
		{
			std::cout << "generating strtab..." << std::endl;
			//+
			//+ header
			//+
			{
				elf64::SectionHeader* header = (elf64::SectionHeader*)headerBuffer;
				header->Type = (uint32_t)elf64::SectionType::SHT_STRTAB;
				header->Flags = 0x00;
				header->Alignment = 0;
				header->OffsetInFile = contentOffset;
				header->VirtualAddress = 0;
				header->SizeInFile = strtabPos;
				header->EntrySize = 0;
				header->Name = 0x01;
				headerOffset+=sizeof(elf64::SectionHeader);
			}
			//+
			//+ content
			//+
			{
				memcpy(contentBuffer,strtabData,strtabPos);
				contentOffset+=strtabPos;
			}
		}
		//,
		//, generate all other section headers and fill in content
		//,
		uint64_t i = 2;
		for(eos& s : outputSections)
		{
			std::cout << "generating section "<<i<<"..." << std::endl;
			//+
			//+ header
			//+
			{
				elf64::SectionHeader* header = (elf64::SectionHeader*)headerBuffer;
				header->Type = (uint32_t)s.type;
				header->Flags = s.Flags;
				header->Alignment = 0;
				header->OffsetInFile = contentOffset;
				header->VirtualAddress = 0;
				header->SizeInFile = s.bytes;
				header->EntrySize = 0;
				if(s.type == elf64::SectionType::SHT_RELA)
				{
					header->EntrySize = sizeof(elf64::RelocationEntry);
					header->Info = i-1;
					header->Link = 0x02;
				}
				else if(s.type == elf64::SectionType::SHT_SYMTAB)
				{
					header->EntrySize = sizeof(elf64::SymbolTableEntry);
					header->Info = 2;
					header->Link = 0x01;
				}
				else
				{
					header->Info = 0;
				}
				header->Name = s.name;
				headerOffset+=sizeof(elf64::SectionHeader);
			}
			//+
			//+ content
			//+
			{
				memcpy(contentBuffer,s.data,s.bytes);
				contentOffset+=s.bytes;
			}
			i++;
		}
		std::cout << "opening file..." << std::endl;
		FILE* f = fopen("./test.elf64","wb");
		std::cout << "writing file..." << std::endl;
		fwrite(fileBuffer,bufferSize,1,f);
		std::cout << "closing file..." << std::endl;
		fclose(f);
		std::cout << "done." << std::endl;
		//,
		//, show outputs
		//,
		{
			std::cout << "\033[33m>>>>>>>>>>>> elflint <<<<<<<<<<<<\033[0m" << std::endl;
			system("eu-elflint test.elf64");
			std::cout << "\033[33m>>>>>>>>>>>> readelf <<<<<<<<<<<<\033[0m" << std::endl;
			system("eu-readelf -a test.elf64");
			std::cout << "\033[33m>>>>>>>>>>>> objdump <<<<<<<<<<<<\033[0m" << std::endl;
			system("objdump -d -M amd64 -M intel -M intel-mnemonic --no-show-raw-insn --visualize-jumps=extended-color --disassembler-color=extended test.elf64");
			std::cout << "\033[33m>>>>>>>>>>>> ld <<<<<<<<<<<<\033[0m" << std::endl;
			system("ld test.elf64 -o test.exe");
			std::cout << "\033[33m>>>>>>>>>>>> objdump <<<<<<<<<<<<\033[0m" << std::endl;
			system("objdump -d -M amd64 -M intel -M intel-mnemonic --no-show-raw-insn --visualize-jumps=extended-color --disassembler-color=extended test.exe");
		}
	}

	void main()
	{
		std::cout << ">>>>>>>>>>>> TEST BUILD RUNNING <<<<<<<<<<<<" << std::endl;
		//.
		//. required sections
		//.
		section* text = new section;
		code = text;
		section* data = new section;
		section* rodata = new section;
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
			cgu::symbols.push_back(cgu::Symbol("__test1",0,0x5000,true));
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
		//.
		//. generate ELF64
		//.
		{
			GenerateSymTabSection();
			addSection(".text",text,elf64::shf::alloc | elf64::shf::execinstr,elf64::SectionType::SHT_PROGBITS);
			finalize();
		}
	}
}