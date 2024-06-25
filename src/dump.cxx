/*
 * Created Date: Saturday May 11th 2024
 * Author: Lilith
 * -----
 * Last Modified: Monday June 3rd 2024 11:44:37 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <dump.hxx>

#include <colors.h>

#define INDENT indent+"   "

#define GENERIC_NULLPTR if(obj == nullptr){std::cout << indent << name << " : "<< COLOR_NULLPTR << "NULLPTR" << COLOR_RESET << std::endl;return;}

#define GENERIC_OBJ_START std::cout << indent << name << " : {" << std::endl;
#define GENERIC_OBJ_END std::cout << indent << "}" << std::endl;

#define COLOR_NULLPTR "\033[31m"
#define COLOR_STRING "\033[34m"
#define COLOR_INT "\033[33m"
#define COLOR_ENUM "\033[36m"
#define COLOR_TRUE "\033[35m"
#define COLOR_FALSE "\033[35m"

class dummy;

//,####################################################################################################################
//,####################################################################################################################
//, ███████ ███    ██ ██    ██ ███    ███                ██       ███████ ████████ ██████  ██ ███    ██  ██████
//, ██      ████   ██ ██    ██ ████  ████                 ██      ██         ██    ██   ██ ██ ████   ██ ██
//, █████   ██ ██  ██ ██    ██ ██ ████ ██     █████ █████  ██     ███████    ██    ██████  ██ ██ ██  ██ ██   ███
//, ██      ██  ██ ██ ██    ██ ██  ██  ██                 ██           ██    ██    ██   ██ ██ ██  ██ ██ ██    ██
//, ███████ ██   ████  ██████  ██      ██                ██       ███████    ██    ██   ██ ██ ██   ████  ██████
//,####################################################################################################################
//,####################################################################################################################
const char* stringify(primitiveOP op) {
    switch (op) {
        case primitiveOP::invalid: return "invalid";
        case primitiveOP::add: return "add";
        case primitiveOP::sub: return "sub";
        case primitiveOP::mul: return "mul";
        case primitiveOP::div: return "div";
        case primitiveOP::mod: return "mod";
        case primitiveOP::assign: return "assign";
        case primitiveOP::_or: return "or";
        case primitiveOP::_xor: return "xor";
        case primitiveOP::_and: return "and";
        case primitiveOP::equal: return "equal";
        case primitiveOP::NotEqual: return "NotEqual";
        case primitiveOP::Greater: return "Greater";
        case primitiveOP::GreaterEqual: return "GreaterEqual";
        case primitiveOP::Less: return "Less";
        case primitiveOP::LessEqual: return "LessEqual";
        case primitiveOP::Not: return "Not";
        case primitiveOP::Inc: return "Inc";
        case primitiveOP::Dec: return "Dec";
        case primitiveOP::Index: return "Index";
        case primitiveOP::Interrupt: return "Interrupt";
        case primitiveOP::CPUid: return "CPUid";
        case primitiveOP::PRINTCHAR: return "PRINTCHAR";
        case primitiveOP::PRINTSTR: return "PRINTSTR";
        case primitiveOP::SYSCALL: return "SYSCALL";
    }
    return "unknown";
}
const char* stringify(scopeType type) {
    switch (type) {
        case scopeType::INVALID: return "INVALID";
        case scopeType::GLOBAL: return "GLOBAL";
        case scopeType::FUNCTION: return "FUNCTION";
        case scopeType::CLASS: return "CLASS";
        case scopeType::NAMESPACE: return "NAMESPACE";
        case scopeType::ENUM: return "ENUM";
        case scopeType::CONDITIONAL_BLOCK: return "CONDITIONAL_BLOCK";
        case scopeType::LOGICAL: return "LOGICAL";
        case scopeType::DUMMY: return "DUMMY";
        case scopeType::TRY_CATCH: return "TRY_CATCH";
        case scopeType::TRY: return "TRY";
        case scopeType::CATCH: return "CATCH";
    }
    return "unknown";
}
//,####################################################################################################################
//,####################################################################################################################
//,  ██████  ███████ ███    ██ ███████ ██████  ██  ██████     ██████   █████  ████████  █████      ████████ ██    ██ ██████  ███████ ███████
//, ██       ██      ████   ██ ██      ██   ██ ██ ██          ██   ██ ██   ██    ██    ██   ██        ██     ██  ██  ██   ██ ██      ██
//, ██   ███ █████   ██ ██  ██ █████   ██████  ██ ██          ██   ██ ███████    ██    ███████        ██      ████   ██████  █████   ███████
//, ██    ██ ██      ██  ██ ██ ██      ██   ██ ██ ██          ██   ██ ██   ██    ██    ██   ██        ██       ██    ██      ██           ██
//,  ██████  ███████ ██   ████ ███████ ██   ██ ██  ██████     ██████  ██   ██    ██    ██   ██        ██       ██    ██      ███████ ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, std::string* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_STRING << "\"" << *obj << "\"" << COLOR_RESET << std::endl;
}
void dump(std::string name, uint64_t* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_INT << *obj << COLOR_RESET << std::endl;
}
void dump(std::string name, int8_t* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_INT << *obj << COLOR_RESET << std::endl;
}
void dump(std::string name, dummy* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_INT << (void*)obj << COLOR_RESET << std::endl;
}
void dump(std::string name, bool* obj, std::string indent)
{
	GENERIC_NULLPTR
	if(*obj)
		std::cout << indent << name << " : "<< COLOR_TRUE << "true" << COLOR_RESET << std::endl;
	else
		std::cout << indent << name << " : "<< COLOR_FALSE << "false" << COLOR_RESET << std::endl;
}
//,####################################################################################################################
//,####################################################################################################################
//, ███████ ███    ██ ██    ██ ███    ███ ███████
//, ██      ████   ██ ██    ██ ████  ████ ██
//, █████   ██ ██  ██ ██    ██ ██ ████ ██ ███████
//, ██      ██  ██ ██ ██    ██ ██  ██  ██      ██
//, ███████ ██   ████  ██████  ██      ██ ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, primitiveOP* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_ENUM << stringify(*obj) << COLOR_RESET << std::endl;
}
void dump(std::string name, scopeType* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_ENUM << stringify(*obj) << COLOR_RESET << std::endl;
}
//,####################################################################################################################
//,####################################################################################################################
//, ███████  ██████  ██████  ██████  ███████
//, ██      ██      ██    ██ ██   ██ ██
//, ███████ ██      ██    ██ ██████  █████
//,      ██ ██      ██    ██ ██      ██
//, ███████  ██████  ██████  ██      ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, scope* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("name", &obj->name, INDENT);
	dump("isIndentBased", &obj->isIndentBased, INDENT);
	dump("templateMode", &obj->templateMode, INDENT);
	dump("leadingSpace", &obj->leadingSpace, INDENT);
	dump("miscData", &obj->miscData, INDENT);
	listdump<variable*>("variables", &obj->variables, INDENT);
	dump("t", &obj->t, INDENT);
	if(obj->t != scopeType::GLOBAL && obj->t != scopeType::NAMESPACE)
		listdump<function*>("functions", &obj->functions, INDENT);
	dump("parent", obj->parent, INDENT);
	dump("lparent", obj->lparent, INDENT);
	dump("cl", obj->cl, INDENT);
	dump("func", obj->func, INDENT);
	vlistdump<token>("attributes", &obj->attribs, INDENT);
	dump("lastReentrySym", &obj->lastReentrySym, INDENT);
	dump("ifCounter", &obj->ifCounter, INDENT);
	dump("elseIfCounter", &obj->elseIfCounter, INDENT);
	dump("elseCounter", &obj->elseCounter, INDENT);
	dump("caseCounter", &obj->caseCounter, INDENT);
	dump("whileCounter", &obj->whileCounter, INDENT);
	dump("forCounter", &obj->forCounter, INDENT);
	dump("tryCounter", &obj->tryCounter, INDENT);
	dump("catchCounter", &obj->catchCounter, INDENT);
	dump("switchCounter", &obj->switchCounter, INDENT);
	dump("conditionalCounter", &obj->conditionalCounter, INDENT);
	dump("booleanReturnCounter", &obj->booleanReturnCounter, INDENT);
	dump("strlenCounter", &obj->strlenCounter, INDENT);
	dump("reentrySymbol", &obj->reentrySymbol, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//, ██    ██  █████  ██████  ██  █████  ██████  ██      ███████
//, ██    ██ ██   ██ ██   ██ ██ ██   ██ ██   ██ ██      ██
//, ██    ██ ███████ ██████  ██ ███████ ██████  ██      █████
//,  ██  ██  ██   ██ ██   ██ ██ ██   ██ ██   ██ ██      ██
//,   ████   ██   ██ ██   ██ ██ ██   ██ ██████  ███████ ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, variable* obj, std::string indent, bool shallow)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("data type", obj->dataType, INDENT);
	dump("name", &obj->name, INDENT);
	dump("symbol", &obj->symbol, INDENT);
	dump("desc", &obj->desc, INDENT);
	dump("doExport", &obj->doExport, INDENT);
	dump("isParameter", &obj->isParameter, INDENT);
	dump("__declared_file", &obj->__declared_file, INDENT);
	dump("__declared_line", &obj->__declared_line, INDENT);
	dump("isStatic", &obj->isStatic, INDENT);
	dump("usedAutoStorage", &obj->usedAutoStorage, INDENT);
	if(!shallow)
		listdump<variable*>("children", &obj->children, INDENT);
	dump("parent", obj->parent, INDENT, true);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//, ███████ ██    ██ ███    ██  ██████ ████████ ██  ██████  ███    ██
//, ██      ██    ██ ████   ██ ██         ██    ██ ██    ██ ████   ██
//, █████   ██    ██ ██ ██  ██ ██         ██    ██ ██    ██ ██ ██  ██
//, ██      ██    ██ ██  ██ ██ ██         ██    ██ ██    ██ ██  ██ ██
//, ██       ██████  ██   ████  ██████    ██    ██  ██████  ██   ████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, function* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("name", &obj->name, INDENT);
	dump("symbol", &obj->symbol, INDENT);
	dump("desc", &obj->desc, INDENT);
	dump("returnDesc", &obj->returnDesc, INDENT);
	dump("returnType", obj->returnType, INDENT);
	dump("miscData", &obj->miscData, INDENT);
	listdump<type*>("parameters", &obj->parameters, INDENT);
	listdump<variable*>("vparams", &obj->vparams, INDENT);
	dump("abi", obj->abi, INDENT);
	dump("__declared_file", &obj->__declared_file, INDENT);
	dump("__declared_line", &obj->__declared_line, INDENT);
	dump("miscData1", (dummy*)obj->miscData1, INDENT);
	dump("isPrimitive", &obj->isPrimitive, INDENT);
	dump("op", &obj->op, INDENT);
	dump("primitiveInPlace", &obj->primitiveInPlace, INDENT);
	dump("primitiveFloat", &obj->primitiveFloat, INDENT);
	dump("isDeprecated", &obj->isDeprecated, INDENT);
	dump("noReturn", &obj->noReturn, INDENT);
	dump("noDoc", &obj->noDoc, INDENT);
	dump("isInline", &obj->isInline, INDENT);
	dump("isLocal", &obj->isLocal, INDENT);
	dump("doExport", &obj->doExport, INDENT);
	dump("ignoreCall", &obj->ignoreCall, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//, ████████ ██    ██ ██████  ███████
//,    ██     ██  ██  ██   ██ ██
//,    ██      ████   ██████  █████
//,    ██       ██    ██      ██
//,    ██       ██    ██      ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, type* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("size", &obj->size, INDENT);
	dump("name", &obj->name, INDENT);
	dump("mangledName", &obj->mangledName, INDENT);
	dump("desc", &obj->desc, INDENT);
	dump("miscData", &obj->miscData, INDENT);
	vlistdump<variable>("members", &obj->members, INDENT);
	vlistdump<function>("functions", &obj->functions, INDENT);
	listdump<type*>("supertypes", &obj->supertypes, INDENT);
	dump("valueType", obj->valueType, INDENT);
	dump("regMode", &obj->regMode, INDENT);
	dump("dwarfID", &obj->dwarfID, INDENT);
	dump("alias", obj->alias, INDENT);
	dump("ExceptionOffset", &obj->ExceptionOffset, INDENT);
	dump("ctor", obj->ctor, INDENT);
	dump("dtor", obj->dtor, INDENT);
	dump("__declared_file", &obj->__declared_file, INDENT);
	dump("__declared_line", &obj->__declared_line, INDENT);
	dump("nodoc", &obj->nodoc, INDENT);
	dump("deprecated", &obj->deprecated, INDENT);
	dump("doExport", &obj->doExport, INDENT);
	dump("iteratable", &obj->iteratable, INDENT);
	dump("incomplete", &obj->incomplete, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//,  ██████  █████  ███████ ████████ ███████ ██    ██ ███    ██  ██████ ████████ ██  ██████  ███    ██
//, ██      ██   ██ ██         ██    ██      ██    ██ ████   ██ ██         ██    ██ ██    ██ ████   ██
//, ██      ███████ ███████    ██    █████   ██    ██ ██ ██  ██ ██         ██    ██ ██    ██ ██ ██  ██
//, ██      ██   ██      ██    ██    ██      ██    ██ ██  ██ ██ ██         ██    ██ ██    ██ ██  ██ ██
//,  ██████ ██   ██ ███████    ██    ██       ██████  ██   ████  ██████    ██    ██  ██████  ██   ████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, castFunction* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("input", obj->input, INDENT);
	dump("output", obj->output, INDENT);
	dump("func", obj->func, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//,  █████  ██████  ██
//, ██   ██ ██   ██ ██
//, ███████ ██████  ██
//, ██   ██ ██   ██ ██
//, ██   ██ ██████  ██
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, ABI* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("name", &obj->name, INDENT);
	dump("genProlouge", (dummy*)obj->genProlouge, INDENT);
	dump("genEpilouge", (dummy*)obj->genEpilouge, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//, ██      ██ ███    ██ ███████
//, ██      ██ ████   ██ ██
//, ██      ██ ██ ██  ██ █████
//, ██      ██ ██  ██ ██ ██
//, ███████ ██ ██   ████ ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, line* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("tpos", &obj->tpos, INDENT);
	dump("ccol", &obj->ccol, INDENT);
	dump("tline", &obj->tline, INDENT);
	dump("whitespace", &obj->whitespace, INDENT);
	dump("twhitespace", &obj->twhitespace, INDENT);
	dump("lineNum", &obj->lineNum, INDENT);
	dump("file", &obj->file, INDENT);
	dump("text", &obj->text, INDENT);
	dump("leadingSpaces", &obj->leadingSpaces, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//, ████████  ██████  ██   ██ ███████ ███    ██
//,    ██    ██    ██ ██  ██  ██      ████   ██
//,    ██    ██    ██ █████   █████   ██ ██  ██
//,    ██    ██    ██ ██  ██  ██      ██  ██ ██
//,    ██     ██████  ██   ██ ███████ ██   ████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, token* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("Line", obj->Line, INDENT);
	dump("text", &obj->text, INDENT);
	dump("col", &obj->col, INDENT);
	dump("tcol", &obj->tcol, INDENT);
	dump("type", &obj->type, INDENT);
	dump("lineNum", &obj->lineNum, INDENT);
	GENERIC_OBJ_END
}

//,####################################################################################################################
//,####################################################################################################################
//, ██      ██ ███████ ████████ ███████
//, ██      ██ ██         ██    ██
//, ██      ██ ███████    ██    ███████
//, ██      ██      ██    ██         ██
//, ███████ ██ ███████    ██    ███████
//,####################################################################################################################
//,####################################################################################################################
template<typename T>
void listdump(std::string name, std::vector<T>* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	uint64_t I = 0;
	for(T& item : *obj)
	{
		dump("["+std::to_string(I++)+"]",item,INDENT);
	}
	GENERIC_OBJ_END
}
template<typename T>
void vlistdump(std::string name, std::vector<T>* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	uint64_t I = 0;
	for(T& item : *obj)
	{
		dump("["+std::to_string(I++)+"]",&item,INDENT);
	}
	GENERIC_OBJ_END
}
