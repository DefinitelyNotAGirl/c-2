#pragma once
#include "class_line.h"
#include "class_token.h"
#include "class_scope.h"
#include <stack>
#include <vector>
#include <string>
#include "EntityManagement.hxx"

#ifdef PARSER_IMPLEMENTATION
	#define global
	#define globalFunction
#else
	#define global extern
	#define globalFunction ;
#endif

/**
 * @brief stores one string for every line that needs to be written to the resource file
 * 
 */
global std::vector<std::string> resourceCode;

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
	void NextToken(){this->Token = this->Line.nextToken();}

	std::stack<scope*> scopes;
	std::vector<line> Lines;
	uint64_t LineIterator = 0;
	dObj* currentd = new dObj;
	std::vector<Entity::Attribute> Attributes;
	std::stack<uint64_t> trycatchSaveallBase;
	struct {
		type* Type = nullptr;
		variable* ArraySize = nullptr;
		token NameToken;
	} DeclarationData;
};
global ParserState_T ParserState;

void resetCurrentD() globalFunction
#ifdef PARSER_IMPLEMENTATION
{
	for(pdobj* i : ParserState.currentd->params)
		delete i;
	for(pdobj* i : ParserState.currentd->tparams)
		delete i;
	delete ParserState.currentd;
	ParserState.currentd = new dObj;
}
#endif

namespace parse
{
	void Lines(std::vector<line>& Lines, std::string File = "");
}
