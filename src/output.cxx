#include <output.hxx>
#include <ELF64.hxx>

std::map<std::string,uint64_t> SymbolMap;
std::map<std::string,uint64_t> ExternalSymbolMap;

section text;
section data;
section rodata;

namespace elf64
{
	section strtab;
	section symtab;
	section symtab_external;
}

namespace dwarf
{
	section debug_info;
	section debug_abbrev;
}

/**
 * @brief helper type to improve initialization performance
 * 
 */
union __emptyspace {
	elf64::SymbolTableEntry elf64_symtab_entry;
};

void output_init()
{
	byte empty[sizeof(__emptyspace)] = {0x00};
	//,
	//, initiate elf64::strtab
	//,
	{
		elf64::strtab.push(empty,1);
	}
	//,
	//, initiate elf64::symtab (local and external)
	//,
	{
		elf64::symtab.push(empty,sizeof(elf64::SymbolTableEntry));
		//. external symbol table does not need a null entry as it gets appended to the local symtab
		//. when the final binary is generated
	}
}
