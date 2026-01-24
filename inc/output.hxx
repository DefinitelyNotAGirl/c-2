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
/**
 * @brief imports an external value for linking
 */
void importExternalValue(std::string name);
/**
 * @brief imports an external function for linking
 */
void importExternalFunction(std::string name);
/**
 * @brief imports an external variable for linking
 */
void importExternalVariable(std::string name);
/**
 * @brief sets a global value for the linker to deal with
 */
void setGlobalValue(std::string name, uint64_t value);
/**
 * @brief sets a local value for the linker to deal with
 */
void setLocalValue(std::string name, uint64_t value);

namespace output {
	/**
	 * @brief generates elf64 output, this should be among the final steps in the compilers lifecycle
	 */
	void generateElf64();
}
