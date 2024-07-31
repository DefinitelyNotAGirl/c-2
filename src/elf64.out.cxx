#include <compiler.h>
#include <SMU.h>
#include <cgu.h>
#include <amd64.cgu.hxx>
#include <ELF64.hxx>
#include <output.hxx>
#include <issues.hxx>
using namespace issues;

//+####################################################################################################################
//+####################################################################################################################
//+ ██    ██  █████  ██████  ██  █████  ██████  ██      ███████ ███████
//+ ██    ██ ██   ██ ██   ██ ██ ██   ██ ██   ██ ██      ██      ██
//+ ██    ██ ███████ ██████  ██ ███████ ██████  ██      █████   ███████
//+  ██  ██  ██   ██ ██   ██ ██ ██   ██ ██   ██ ██      ██           ██
//+   ████   ██   ██ ██   ██ ██ ██   ██ ██████  ███████ ███████ ███████
//+####################################################################################################################
//+####################################################################################################################
static uint64_t sh_count;
static uint64_t ph_count;
static uint64_t bufferSize;
static section strtab;
static section symtab;
static std::map<std::string,uint64_t> SymbolMap;

//+####################################################################################################################
//+####################################################################################################################
//+ ███████  ██████  ███████
//+ ██      ██    ██ ██
//+ █████   ██    ██ ███████
//+ ██      ██    ██      ██
//+ ███████  ██████  ███████
//+####################################################################################################################
//+####################################################################################################################
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

//+####################################################################################################################
//+####################################################################################################################
//+  ██████  ███████ ███    ██     ██████  ███████ ██       █████      ███████ ███████  ██████ ████████ ██  ██████  ███    ██
//+ ██       ██      ████   ██     ██   ██ ██      ██      ██   ██     ██      ██      ██         ██    ██ ██    ██ ████   ██
//+ ██   ███ █████   ██ ██  ██     ██████  █████   ██      ███████     ███████ █████   ██         ██    ██ ██    ██ ██ ██  ██
//+ ██    ██ ██      ██  ██ ██     ██   ██ ██      ██      ██   ██          ██ ██      ██         ██    ██ ██    ██ ██  ██ ██
//+  ██████  ███████ ██   ████     ██   ██ ███████ ███████ ██   ██     ███████ ███████  ██████    ██    ██  ██████  ██   ████
//+####################################################################################################################
//+####################################################################################################################
static void addSection(std::string name, section* s,uint64_t Flags,elf64::SectionType type,bool generateRela);
static void GenerateRelaSection(std::string name,section* s)
{
	section* rs = new section;
	for(smu::RelocationEntry& re : s->Relocations)
	{
		uint64_t symNum;
		try {
			symNum = SymbolMap.at(re.symbol);
		}
		catch(std::out_of_range e)
		{
			compilerBug("symbol not found, "+re.symbol);
		}
		elf64::RelocationType rtype;
		if(re.type == smu::RelocationType::Absolute && re.size == 8)
			rtype = elf64::RelocationType::R_X86_64_64;
		else if(re.type == smu::RelocationType::Absolute && re.size == 4)
			rtype = elf64::RelocationType::R_X86_64_32;
		else if(re.type == smu::RelocationType::Relative && re.size == 8)
			rtype = elf64::RelocationType::R_X86_64_RELATIVE64;
		else if(re.type == smu::RelocationType::Relative && re.size == 4)
			rtype = elf64::RelocationType::R_X86_64_PC32;
		else
			compilerBug("unimplemented, relocation: "+std::to_string((uint64_t)re.type)+" - "+std::to_string(re.size));
		rs->push(elf64::RelocationEntry(re.offset,symNum,rtype,0));
	}
	addSection(name,rs,0x00,elf64::SectionType::SHT_RELA,false);
}

//+####################################################################################################################
//+####################################################################################################################
//+  █████  ██████  ██████      ███████ ███████  ██████ ████████ ██  ██████  ███    ██
//+ ██   ██ ██   ██ ██   ██     ██      ██      ██         ██    ██ ██    ██ ████   ██
//+ ███████ ██   ██ ██   ██     ███████ █████   ██         ██    ██ ██    ██ ██ ██  ██
//+ ██   ██ ██   ██ ██   ██          ██ ██      ██         ██    ██ ██    ██ ██  ██ ██
//+ ██   ██ ██████  ██████      ███████ ███████  ██████    ██    ██  ██████  ██   ████
//+####################################################################################################################
//+####################################################################################################################
static void addSection(std::string name, section* s,uint64_t Flags,elf64::SectionType type,bool generateRela)
{
	if(s->size() == 0)
	{
		std::cout << "empty section: " << name << std::endl;
		//return;
	}
	uint64_t namePos = 0;
	if(name != "")
	{
		namePos = strtab.size();
		strtab.push(name.data(),name.length()+1);
	}
	bufferSize+=sizeof(elf64::SectionHeader);
	bufferSize+=s->size();
	sh_count++;
	outputSections.push_back(eos(s->size(),s->data,type,Flags,namePos));
	if(generateRela)
		GenerateRelaSection(".rela"+name,s);
}
//+####################################################################################################################
//+####################################################################################################################
//+  ██████  ███████ ███    ██     ███████ ██    ██ ███    ███ ████████  █████  ██████
//+ ██       ██      ████   ██     ██       ██  ██  ████  ████    ██    ██   ██ ██   ██
//+ ██   ███ █████   ██ ██  ██     ███████   ████   ██ ████ ██    ██    ███████ ██████
//+ ██    ██ ██      ██  ██ ██          ██    ██    ██  ██  ██    ██    ██   ██ ██   ██
//+  ██████  ███████ ██   ████     ███████    ██    ██      ██    ██    ██   ██ ██████
//+####################################################################################################################
//+####################################################################################################################
void genLocalSymtab(section* sec)
{
	for(Symbol& s : sec->symbols)
	{
		//.
		//. check if symbol is local
		//.
		{
			if((((uint64_t)s.Type) & 0xF0) == 0x10)continue;
			if((((uint64_t)s.Type) & 0xF0) == 0x30)continue;
		}
		elf64::SymbolTableEntry e;
		e.name = strtab.size();
		e.other = 0x00;
		e.SymbolSize = s.size;
		e.SymbolValue = s.value;
		SymbolMap.insert({std::pair<std::string,uint64_t>(s.name,symtab.size()/sizeof(elf64::SymbolTableEntry))});
		switch(s.Type)
		{
			case(SymbolType::CodeLocation):
			{
				e.info = 0x00;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::TEXT;
				break;
			}
			case(SymbolType::LocalFunction):
			{
				e.info = 0x02;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::TEXT;
				break;
			}
			case(SymbolType::LocalVariable):
			{
				e.info = 0x01;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::DATA;
				break;
			}
			default:
				compilerBug("unknown symbol type: "+std::to_string((uint64_t)s.Type)+" ("+s.name+")");
		}
		//.
		//. push name to strtab
		//.
		{
			strtab.push(
				s.name.data(),
				s.name.length()+1
			);
		}
		symtab.push(e);
	}
}
void genGlobalSymtab(section* sec)
{
	for(Symbol& s : sec->symbols)
	{	
		//.
		//. check if symbol is local
		//.
		{
			if((((uint64_t)s.Type) & 0xF0) == 0x20)continue;
			if((((uint64_t)s.Type) & 0xF0) == 0x00)continue;
		}
		elf64::SymbolTableEntry e;
		e.name = strtab.size();
		e.other = 0x00;
		e.SymbolSize = s.size;
		e.SymbolValue = s.value;
		SymbolMap.insert({std::pair<std::string,uint64_t>(s.name,symtab.size()/sizeof(elf64::SymbolTableEntry))});
		switch(s.Type)
		{
			case(SymbolType::GlobalFunction):
			{
				e.info = 0x12;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::TEXT;
				break;
			}
			case(SymbolType::GlobalVariable):
			{
				e.info = 0x11;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::DATA;
				break;
			}
			case(SymbolType::ExternalFunction):
			{
				e.info = 0x22;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::UNDEF;
				break;
			}
			case(SymbolType::ExternalVariable):
			{
				e.info = 0x21;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::UNDEF;
				break;
			}
			default:
				compilerBug("unknown symbol type: "+std::to_string((uint64_t)s.Type)+" ("+s.name+")");
		}
		//.
		//. push name to strtab
		//.
		{
			strtab.push(
				s.name.data(),
				s.name.length()+1
			);
		}
		symtab.push(e);
	}
}
void genLocalValueSymtab()
{
	for(Symbol& s : ValueSymbols)
	{
		
		//.
		//. check if symbol is local
		//.
		{
			if((((uint64_t)s.Type) & 0xF0) == 0x10)continue;
			if((((uint64_t)s.Type) & 0xF0) == 0x30)continue;
		}
		elf64::SymbolTableEntry e;
		e.name = strtab.size();
		e.other = 0x00;
		e.SymbolSize = s.size;
		e.SymbolValue = s.value;
		SymbolMap.insert({std::pair<std::string,uint64_t>(s.name,symtab.size()/sizeof(elf64::SymbolTableEntry))});
		switch(s.Type)
		{
			case(SymbolType::LocalValue):
			{
				e.info = 0x00;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::ABS;
				break;
			}
			default:
				compilerBug("unknown symbol type: "+std::to_string((uint64_t)s.Type)+" ("+s.name+")");
		}
		//.
		//. push name to strtab
		//.
		{
			strtab.push(
				s.name.data(),
				s.name.length()+1
			);
		}
		symtab.push(e);
	}
}
void genGlobalValueSymtab()
{
	for(Symbol& s : ValueSymbols)
	{
		//.
		//. check if symbol is local
		//.
		{
			if((((uint64_t)s.Type) & 0xF0) == 0x20)continue;
			if((((uint64_t)s.Type) & 0xF0) == 0x00)continue;
		}
		elf64::SymbolTableEntry e;
		e.name = strtab.size();
		e.other = 0x00;
		e.SymbolSize = s.size;
		e.SymbolValue = s.value;
		SymbolMap.insert({std::pair<std::string,uint64_t>(s.name,symtab.size()/sizeof(elf64::SymbolTableEntry))});
		switch(s.Type)
		{
			case(SymbolType::GlobalValue):
			{
				e.info = 0x10;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::ABS;
				break;
			}
			case(SymbolType::ExternalValue):
			{
				e.info = 0x10;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::UNDEF;
				break;
			}
			case(SymbolType::ExternalVariable):
			{
				e.info = 0x11;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::UNDEF;
				break;
			}
			case(SymbolType::ExternalFunction):
			{
				e.info = 0x12;
				e.SectionTableIndex = (uint64_t)elf64::SectionTableIndex::UNDEF;
				break;
			}
			default:
				compilerBug("unknown symbol type: "+std::to_string((uint64_t)s.Type)+" ("+s.name+")");
		}
		//.
		//. push name to strtab
		//.
		{
			strtab.push(
				s.name.data(),
				s.name.length()+1
			);
		}
		symtab.push(e);
	}
}
//+####################################################################################################################
//+####################################################################################################################
//+  ██████  ███████ ███    ██     ███████ ██      ███████  ██████  ██   ██
//+ ██       ██      ████   ██     ██      ██      ██      ██       ██   ██
//+ ██   ███ █████   ██ ██  ██     █████   ██      █████   ███████  ███████
//+ ██    ██ ██      ██  ██ ██     ██      ██      ██      ██    ██      ██
//+  ██████  ███████ ██   ████     ███████ ███████ ██       ██████       ██
//+####################################################################################################################
//+####################################################################################################################
#define contentBuffer ((void*)((byte*)fileBuffer+contentOffset))
#define headerBuffer ((void*)((byte*)fileBuffer+headerOffset))
extern bool emitExceptionSymbols;
extern uint64_t exceptionoffset;
union __emptyspace {
	elf64::SymbolTableEntry elf64_symtab_entry;
};
void output::generateElf64()
{
	//,
	//, reset variables
	//,
	uint64_t sh_count_initial = 1;
	uint64_t ph_count_initial = 0;
	{
		sh_count = sh_count_initial;
		ph_count = ph_count_initial;
		bufferSize = sizeof(elf64::Header)+(sh_count_initial*sizeof(elf64::SectionHeader));
	}
	//,
	//, push null entries
	//,
	{
		byte empty[sizeof(__emptyspace)] = {0x00};
		strtab.push(empty,1);
		symtab.push(empty,sizeof(elf64::SymbolTableEntry));
	}
	//,
	//, generate symbol tables
	//,
	uint64_t LocalSymbolCount;
	{
		genLocalSymtab(&text);
		genLocalSymtab(&data);
		genLocalSymtab(&rodata);
		genLocalValueSymtab();
		LocalSymbolCount = symtab.size()/sizeof(elf64::SymbolTableEntry);
		genGlobalSymtab(&text);
		genGlobalSymtab(&data);
		genGlobalSymtab(&rodata);
		genGlobalValueSymtab();
	}
	//,
	//, get sections
	//,
	uint64_t index_symtab;
	uint64_t index_strtab;
	{
		index_symtab = sh_count;
		addSection(".symtab",&symtab,0x00,elf64::SectionType::SHT_SYMTAB,false);
		addSection(".text",&text,elf64::shf::alloc | elf64::shf::execinstr,elf64::SectionType::SHT_PROGBITS,true);
		addSection(".data",&data,elf64::shf::alloc | elf64::shf::write,elf64::SectionType::SHT_PROGBITS,true);
		addSection(".rodata",&rodata,elf64::shf::alloc,elf64::SectionType::SHT_PROGBITS,true);
		index_strtab = sh_count;
		addSection(".strtab",&strtab,0x00,elf64::SectionType::SHT_STRTAB,false);
	}
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
		header->SectionNameEntry = index_strtab;
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
	//, generate all generic section headers and fill in content
	//,
	uint64_t i = sh_count_initial;
	for(eos& s : outputSections)
	{
		std::cout << "generating section "<<strtab.data+s.name<<"..." << std::endl;
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
			header->Link = 0x00;
			header->Info = 0x00;
			if(s.type == elf64::SectionType::SHT_RELA)
			{
				header->EntrySize = sizeof(elf64::RelocationEntry);
				header->Info = i-1;
				header->Link = index_symtab;
			}
			else if(s.type == elf64::SectionType::SHT_SYMTAB)
			{
				header->EntrySize = sizeof(elf64::SymbolTableEntry);
				header->Info = LocalSymbolCount;
				header->Link = index_strtab;
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
	//std::cout << "opening file: " << objOut << std::endl;
	FILE* f = fopen(objOut.c_str(),"wb");
	//std::cout << "writing file..." << std::endl;
	fwrite(fileBuffer,bufferSize,1,f);
	//std::cout << "closing file..." << std::endl;
	fclose(f);
	//std::cout << "done." << std::endl;
	//,
	//, show outputs
	//,
	if(false){
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
