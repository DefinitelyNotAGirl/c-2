#pragma once
#include <string>
#include <vector>

namespace cgu
{
	class Symbol {
	public:
		uint64_t SectionIndex;
		std::string text;
		uint64_t size;
		uint64_t offset;
		bool defined;
		Symbol(std::string text, uint64_t size, uint64_t offset, bool defined, uint64_t SectionIndex = 0xFFF1)
			:text(text),size(size),offset(offset),defined(defined),SectionIndex(SectionIndex){}
	};

	std::vector<Symbol> symbols;
}
