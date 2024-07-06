#pragma once
#include <class_line.h>
#include <class_token.h>
#include <class_scope.h>
#include <stack>
#include <vector>
#include <string>

class ParserState_T
{
public:
	std::string File;
	line Line;
	token Token;

	std::stack<scope*> scope;
	std::vector<line> Lines;
	uint64_t LineIterator = 0;
};

extern ParserState_T ParserState;

class dummy_endline{uint64_t __dummycontent;};
#define endline ((dummy_endline*)0)
class dummy_endparse{uint64_t __dummycontent;};
#define endparse ((dummy_endparse*)0)

namespace parse {
	void Lines(std::vector<line>& Lines, std::string File = "");
	void Line();
	void Directive();
	namespace Directives {
		void Autodecl();
	}
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
	namespace Keyword {
		void Return();
		void While();
		void For();
		void If();
		void Else();
		void ElseIf();
		void Switch();
		void Case();
		void Break();
		void Try();
		void Catch();
		void Throw();
		void Async();
		void Template();
	}
}
