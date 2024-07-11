#include <class_line.h>
#include <class_token.h>
#include <class_scope.h>
#include <stack>
#include <vector>
#include <string>
#include <EntityManagement.hxx>

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

class dObj;
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
	void NextToken(){this->Token = this->Line.nextToken();}

	std::stack<scope*> scope;
	std::vector<line> Lines;
	uint64_t LineIterator = 0;

	dObj* currentd = new dObj;

	std::vector<Entity::Attribute> Attributes;

	struct {
		type* Type = nullptr;
		variable* ArraySize = nullptr;
		token NameToken;
	} DeclarationData;
};
global ParserState_T ParserState;

namespace parse
{
	void Lines(std::vector<line>& Lines, std::string File = "");
}
