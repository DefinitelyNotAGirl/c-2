#include "SMU.h"
#include "ELF64.hxx"

extern section text;
extern section data;
extern section rodata;
namespace dwarf
{
	extern section debug_info;
	extern section debug_abbrev;
}

extern std::vector<Symbol> ValueSymbols;
void importExternalValue(std::string name);
void importExternalFunction(std::string name);
void importExternalVariable(std::string name);
void setGlobalValue(std::string name, uint64_t value);
void setLocalValue(std::string name, uint64_t value);

namespace output {
	void generateElf64();
}
