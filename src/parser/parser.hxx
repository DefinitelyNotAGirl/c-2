#pragma once
#include <class_line.h>
#include <class_token.h>
#include <class_scope.h>
#include <stack>
#include <vector>
#include <string>
#include <EntityManagement.hxx>
#include <issues.hxx>
using namespace issues;

#undef PARSER_IMPLEMENTATION
#ifdef PARSER_IMPLEMENTATION
	#define global
	#define globalFunction \
	#if true
#else
	#define global extern
	#define globalFunction ;\
	#if false
#endif

//. ██████  ███████ ███████  ██████ ██████  ██ ██████  ████████  ██████  ██████       ██████  ██████       ██ ███████  ██████ ████████ ███████
//. ██   ██ ██      ██      ██      ██   ██ ██ ██   ██    ██    ██    ██ ██   ██     ██    ██ ██   ██      ██ ██      ██         ██    ██
//. ██   ██ █████   ███████ ██      ██████  ██ ██████     ██    ██    ██ ██████      ██    ██ ██████       ██ █████   ██         ██    ███████
//. ██   ██ ██           ██ ██      ██   ██ ██ ██         ██    ██    ██ ██   ██     ██    ██ ██   ██ ██   ██ ██      ██         ██         ██
//. ██████  ███████ ███████  ██████ ██   ██ ██ ██         ██     ██████  ██   ██      ██████  ██████   █████  ███████  ██████    ██    ███████
class pdobj
{
public:
	std::string name;
	std::string desc;
};

class dObj
{
public:
	std::string desc;
	std::string ret;
	std::vector<pdobj*> params;
	std::vector<pdobj*> tparams;
};

//. ██████   █████  ██████  ███████ ███████ ██████      ███████ ████████  █████  ████████ ███████
//. ██   ██ ██   ██ ██   ██ ██      ██      ██   ██     ██         ██    ██   ██    ██    ██
//. ██████  ███████ ██████  ███████ █████   ██████      ███████    ██    ███████    ██    █████
//. ██      ██   ██ ██   ██      ██ ██      ██   ██          ██    ██    ██   ██    ██    ██
//. ██      ██   ██ ██   ██ ███████ ███████ ██   ██     ███████    ██    ██   ██    ██    ███████
class ParserState_T
{
public:
	std::string File;
	line Line;
	token Token;

	std::stack<scope*> scope;
	std::vector<line> Lines;
	uint64_t LineIterator = 0;

	dObj* currentd = new dObj;

	std::vector<Entity::Attribute> Attributes;
};
global ParserState_T ParserState;

void resetCurrentD() globalFunction
{
	for(pdobj* i : ParserState.currentd->params)
		delete i;
	for(pdobj* i : ParserState.currentd->tparams)
		delete i;
	delete ParserState.currentd;
	ParserState.currentd = new dObj;
}
#endif



class dummy_parser_endline{};
class dummy_parser_endblock{};

namespace parse {
	inline void EndLine(){throw ((dummy_parser_endline*)0);}
	inline void EndBlock(){throw ((dummy_parser_endblock*)0);}

	void Lines(std::vector<line>& Lines, std::string File = "");
	void Line();
	void Directive();
	namespace Directives {
		void Autodecl();
		void Include();
		void Pragma();
	}

	void Expression();

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
