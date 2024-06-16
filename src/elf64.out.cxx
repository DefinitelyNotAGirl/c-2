#include <compiler.h>
#include <SMU.h>
#include <cgu.h>
#include <amd64.cgu.hxx>
#include <ELF64.hxx>
#include <output.hxx>
#include <issues.hxx>
using namespace issues;

template<typename T>
void memdump(uint64_t start,uint64_t size,uint64_t blocksPerLine)
{
	std::cout << "memory dump, block size: " << (uint64_t)sizeof(T) << ", data size: " << size << std::endl;
	//,
	//, int to string buffer
	//,
	//uint64_t bufferSize = (sizeof(T)*2)+1;
	uint64_t bufferSize = 2+1;
	char buffer[bufferSize];
	buffer[bufferSize-1] = 0;
	uint64_t textBufferSize = (sizeof(T)*blocksPerLine)+4;
	char textBuffer[textBufferSize];
	for(uint64_t i = 0;i<textBufferSize;i++)
		textBuffer[i] = ' ';
	textBuffer[textBufferSize-1] = 0;
	textBuffer[textBufferSize-2] = ']';
	textBuffer[1] = '[';
	textBuffer[0] = ' ';
	//,
	//, HEXDIG buffer
	//,
	char HEXDIG[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
	char ASCII[256] = {
		//? control characters
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',//0-15
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',//16-31
		//? printable characters
		' ','!','"','#','$','%','&','\'','(',')','*','+',',','-','.','/',
		'0','1','2','3','4','5','6','7','8','9',':',';','<','=','>','?',
		'@','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
		'P','Q','R','S','T','U','V','W','X','Y','Z','[','\\',']','^','_',
		'`','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o',
		'p','q','r','s','t','u','v','w','x','y','z','{','|','}','~','.',
		//? extended
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.',
		'.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.'
	};
	//,
	//, dump data
	//,
	uint64_t BlockLine = 0;
	uint8_t* dataArray = (uint8_t*)(start);
	uint64_t c = 0;
	for(uint64_t I = 0;I<size;I++)
	{
		if(I!=0 && ((I%sizeof(T))==0))
		{
			std::cout << ' ';
			BlockLine++;
		}
		uint8_t data = dataArray[I];
		buffer[1] = HEXDIG[((data&0xF))];
		buffer[0] = HEXDIG[((data&0xF0)>>4)];
		if(BlockLine == blocksPerLine)
		{
			c = 0;
			std::cout << textBuffer;
			std::cout << std::endl;
			BlockLine = 0;
		}
		textBuffer[(2)+(c)] = ASCII[data];
		c++;
		std::cout << buffer;
	}
	uint64_t mod = size%(sizeof(T)*blocksPerLine);
	mod+=(mod/2);
	mod--;
	for(uint64_t I = 0;I<mod;I++)
		std::cout << ' ';
	std::cout << ' ' << textBuffer;
	std::cout << std::endl;
}

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
static uint64_t LocalSymbolCount;

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
static void addSection(std::string name, section* s,uint64_t Flags,elf64::SectionType type);
static void GenerateRelaSection(std::string name,section* s)
{
	section* rs = new section;
	for(smu::RelocationEntry& re : s->Relocations)
	{
		auto it = SymbolMap.find(re.symbol);
		if(it == SymbolMap.end())
		{
			compilerBug("symbol not found, "+re.symbol);
		}
		uint64_t symNum = SymbolMap.extract(it).mapped();
		elf64::RelocationType rtype;
		if(re.type == smu::RelocationType::Absolute && re.size == 8)
			rtype = elf64::RelocationType::R_X86_64_64;
		else
			compilerBug("unimplemented");
		rs->push(elf64::RelocationEntry(re.offset,symNum,rtype,0));
	}
	for(smu::RelocationEntry& re : s->ExternalRelocations)
	{
		auto it = ExternalSymbolMap.find(re.symbol);
		if(it == ExternalSymbolMap.end())
		{
			compilerBug("symbol not found, "+re.symbol);
		}
		uint64_t symNum = SymbolMap.extract(it).mapped()+LocalSymbolCount;
		elf64::RelocationType rtype;
		if(re.type == smu::RelocationType::Absolute && re.size == 8)
			rtype = elf64::RelocationType::R_X86_64_64;
		else
			compilerBug("unimplemented");
		rs->push(elf64::RelocationEntry(re.offset,symNum,rtype,0));
	}
	addSection(name,rs,0x00,elf64::SectionType::SHT_RELA);
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
static void addSection(std::string name, section* s,uint64_t Flags,elf64::SectionType type)
{
	if(s->size() == 0)
		return;
	uint64_t namePos = 0;
	if(name != "")
	{
		namePos = elf64::strtab.size();
		elf64::strtab.push(name.data(),name.length()+1);
	}
	bufferSize+=sizeof(elf64::SectionHeader);
	bufferSize+=s->size();
	sh_count++;
	outputSections.push_back(eos(s->size(),s->data,type,Flags,namePos));
	if(s->Relocations.size() > 0)
		GenerateRelaSection(".rela"+name,s);
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
#define contentBuffer (fileBuffer+contentOffset)
#define headerBuffer (fileBuffer+headerOffset)
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
		LocalSymbolCount = 0;
	}
	//,
	//, merge symbol tables
	//,
	{
		LocalSymbolCount = (elf64::symtab.size()/sizeof(elf64::SymbolTableEntry));
		if(elf64::symtab_external.size() > 0)
			elf64::symtab.push(elf64::symtab_external);
	}
	//,
	//, get sections
	//,
	uint64_t index_symtab;
	uint64_t index_strtab;
	{
		index_symtab = sh_count;
		addSection(".symtab",&elf64::symtab,0x00,elf64::SectionType::SHT_SYMTAB);
		addSection(".text",&text,elf64::shf::alloc | elf64::shf::execinstr,elf64::SectionType::SHT_PROGBITS);
		addSection(".data",&data,elf64::shf::alloc | elf64::shf::write,elf64::SectionType::SHT_PROGBITS);
		addSection(".rodata",&rodata,elf64::shf::alloc,elf64::SectionType::SHT_PROGBITS);
		index_strtab = sh_count;
		addSection(".strtab",&elf64::strtab,0x00,elf64::SectionType::SHT_STRTAB);
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
