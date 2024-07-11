#pragma once
#include <Parser.hxx>
#include <issues.hxx>
using namespace issues;

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

	void Keyword();
	namespace Keywords {
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
