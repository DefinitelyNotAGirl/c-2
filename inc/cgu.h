#pragma once
#include <string>
#include <vector>

namespace cgu
{
	class Symbol {
		std::string text;
		uint64_t size;
		uint64_t offset;
		bool defined;
	};

	std::vector<Symbol> symbols;
}
