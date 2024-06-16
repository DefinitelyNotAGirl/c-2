#include <SMU.h>

extern std::map<std::string,uint64_t> SymbolMap;
extern std::map<std::string,uint64_t> ExternalSymbolMap;

extern section text;
extern section data;
extern section rodata;

namespace elf64
{
	extern section strtab;
	extern section symtab;
	extern section symtab_external;
}

namespace dwarf
{
	extern section debug_info;
	extern section debug_abbrev;
}

namespace output {
	void generateElf64();
}
