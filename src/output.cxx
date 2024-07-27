#include <output.hxx>
#include <ELF64.hxx>

section text;
section data;
section rodata;

std::vector<Symbol> ValueSymbols;
void importExternalFunction(std::string name)
{
	ValueSymbols.push_back(
		Symbol(
			SymbolType::ExternalFunction,
			0,
			name
		)
	);
}
void importExternalVariable(std::string name)
{
	ValueSymbols.push_back(
		Symbol(
			SymbolType::ExternalVariable,
			0,
			name
		)
	);
}
void importExternalValue(std::string name)
{
	ValueSymbols.push_back(
		Symbol(
			SymbolType::ExternalValue,
			0,
			name
		)
	);
}
void setGlobalValue(std::string name, uint64_t value)
{
	std::cout << "setting global value: " << name << " to " << value << std::endl;
	ValueSymbols.push_back(
		Symbol(
			SymbolType::GlobalValue,
			value,
			name
		)
	);
}
void setLocalValue(std::string name, uint64_t value)
{
	ValueSymbols.push_back(
		Symbol(
			SymbolType::LocalValue,
			value,
			name
		)
	);
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
}
