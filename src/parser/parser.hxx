#pragma once
#include <Parser.hxx>
#include <issues.hxx>
using namespace issues;

class dummy_parser_endline{};
class dummy_parser_endblock{};

global std::vector<std::string> includedFiles;
global std::vector<std::string> dependencies;
#ifdef PARSER_IMPLEMENTATION
	uint64_t nextExceptionTypeOffset = 8;
#else
	extern uint64_t nextExceptionTypeOffset;
#endif

/**
 * @brief stores one string for every line that needs to be written to the resource file
 * 
 */
global std::vector<std::string> resourceCode;
/**
 * @brief stores a list of all symbols defined using the c2resource keyword
 * 
 */
global std::vector<std::string> resourceSymbols;
#ifdef PARSER_IMPLEMENTATION
	uint64_t exceptionoffset = 16;
	bool emitExceptionSymbols = false;
#else
	extern uint64_t exceptionoffset;
	extern bool emitExceptionSymbols;
#endif

namespace parse {
	inline void EndLine(){throw ((dummy_parser_endline*)0);}
	inline void EndBlock(){throw ((dummy_parser_endblock*)0);}

	void Line();
	void Directive();
	namespace Directives {
		void Autodecl();
		void Include();
		void Pragma();
	}

	void Expression();
	void Call();

	void NewUnique();

	void Descriptor();
	void AttributeList();
	namespace Declaration {
		void FunctionOrVariable();
		void Variable();
		void Function();
		void Class();
		void Enum();
		void Namespace();
		void Litop();
	}

	void Keyword();
	namespace Keywords {
		void Return();
		void While();
		void For();
		void If();
		void Else();
		void ElseIf();
		inline void Switch(){compilerBug("keyword: 'Switch' not implemented");}
		inline void Case(){compilerBug("keyword: 'Case' not implemented");}
		inline void Break(){compilerBug("keyword: 'Break' not implemented");}
		void Try();
		void Catch();
		void Throw();
		void Async();
		inline void Template(){compilerBug("keyword: 'Template' not implemented");};
		void c2Resource();
	}
}
