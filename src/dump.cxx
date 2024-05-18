/*
 * Created Date: Saturday May 11th 2024
 * Author: Lilith
 * -----
 * Last Modified: Saturday May 11th 2024 5:02:54 am
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
const char* stringify(__register__ reg) {
    switch (reg) {
        case __register__::invalid: return "invalid";
        case __register__::rax: return "rax";
        case __register__::rbx: return "rbx";
        case __register__::rcx: return "rcx";
        case __register__::rdx: return "rdx";
        case __register__::rsi: return "rsi";
        case __register__::rdi: return "rdi";
        case __register__::rbp: return "rbp";
        case __register__::rsp: return "rsp";
        case __register__::r0: return "r0";
        case __register__::r1: return "r1";
        case __register__::r2: return "r2";
        case __register__::r3: return "r3";
        case __register__::r4: return "r4";
        case __register__::r5: return "r5";
        case __register__::r6: return "r6";
        case __register__::r7: return "r7";
        case __register__::r8: return "r8";
        case __register__::r9: return "r9";
        case __register__::r10: return "r10";
        case __register__::r11: return "r11";
        case __register__::r12: return "r12";
        case __register__::r13: return "r13";
        case __register__::r14: return "r14";
        case __register__::r15: return "r15";
        case __register__::rip: return "rip";
        case __register__::cr0: return "cr0";
        case __register__::cr1: return "cr1";
        case __register__::cr2: return "cr2";
        case __register__::cr3: return "cr3";
        case __register__::cr4: return "cr4";
        case __register__::cr5: return "cr5";
        case __register__::cr6: return "cr6";
        case __register__::cr7: return "cr7";
        case __register__::cr8: return "cr8";
        case __register__::cr9: return "cr9";
        case __register__::cr10: return "cr10";
        case __register__::cr11: return "cr11";
        case __register__::cr12: return "cr12";
        case __register__::cr13: return "cr13";
        case __register__::cr14: return "cr14";
        case __register__::cr15: return "cr15";
        case __register__::EFER: return "EFER";
        case __register__::DR0: return "DR0";
        case __register__::DR1: return "DR1";
        case __register__::DR2: return "DR2";
        case __register__::DR3: return "DR3";
        case __register__::DR6: return "DR6";
        case __register__::DR7: return "DR7";
        case __register__::GDTR: return "GDTR";
        case __register__::IDTR: return "IDTR";
        case __register__::LDTR: return "LDTR";
        case __register__::TR: return "TR";
        case __register__::CS: return "CS";
        case __register__::DS: return "DS";
        case __register__::SS: return "SS";
        case __register__::ES: return "ES";
        case __register__::FS: return "FS";
        case __register__::GS: return "GS";
        case __register__::xmm0: return "xmm0";
        case __register__::xmm1: return "xmm1";
        case __register__::xmm2: return "xmm2";
        case __register__::xmm3: return "xmm3";
        case __register__::xmm4: return "xmm4";
        case __register__::xmm5: return "xmm5";
        case __register__::xmm6: return "xmm6";
        case __register__::xmm7: return "xmm7";
        case __register__::xmm8: return "xmm8";
        case __register__::xmm9: return "xmm9";
        case __register__::xmm10: return "xmm10";
        case __register__::xmm11: return "xmm11";
        case __register__::xmm12: return "xmm12";
        case __register__::xmm13: return "xmm13";
        case __register__::xmm14: return "xmm14";
        case __register__::xmm15: return "xmm15";
    }
    return "unknown";
}
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
const char* stringify(storageType type) {
    switch (type) {
        case storageType::INVALID: return "INVALID";
        case storageType::REGISTER: return "REGISTER";
        case storageType::MEMORY: return "MEMORY";
        case storageType::MEMORY_ABSOLUTE: return "MEMORY_ABSOLUTE";
        case storageType::IMMEDIATE: return "IMMEDIATE";
        case storageType::SYMBOL: return "SYMBOL";
        case storageType::SYMBOL_ADDR: return "SYMBOL_ADDR";
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
void dump(std::string name, __register__* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_ENUM << stringify(*obj) << COLOR_RESET << std::endl;
}
void dump(std::string name, primitiveOP* obj, std::string indent)
{
	GENERIC_NULLPTR
	std::cout << indent << name << " : "<< COLOR_ENUM << stringify(*obj) << COLOR_RESET << std::endl;
}
void dump(std::string name, storageType* obj, std::string indent)
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
	dump("fstore", obj->fstore, INDENT);
	vlistdump<token>("attributes", &obj->attribs, INDENT);
	//,
	//, dump extra code blocks
	//,
	std::string ogin = indent;
	{
		indent = INDENT;
		name = "extraCodeBlocks";
		GENERIC_OBJ_START
		uint64_t I = 0;
		for(std::vector<std::string>* block : obj->extraCodeBlocks)
		{
			vlistdump("["+std::to_string(I++)+"]",block,INDENT);
		}
		GENERIC_OBJ_END
	}
	indent = ogin;
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
	dump("access", &obj->access, INDENT);
	dump("isStatic", &obj->isStatic, INDENT);
	dump("usedAutoStorage", &obj->usedAutoStorage, INDENT);
	dump("immediateValue", &obj->immediateValue, INDENT);
	if(!shallow)
		listdump<variable*>("children", &obj->children, INDENT);
	dump("parent", obj->parent, INDENT, true);
	dump("storage", &obj->storage, INDENT);
	dump("offsetType", &obj->offsetType, INDENT);
	dump("offsetReg", &obj->offsetReg, INDENT);
	dump("reg", &obj->reg, INDENT);
	dump("offset", &obj->offset, INDENT);
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
	vlistdump("code", &obj->code, INDENT);
	dump("abi", obj->abi, INDENT);
	dump("fstore", obj->fstore, INDENT);
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
//,  █████  ██████   ██████ ██   ██
//, ██   ██ ██   ██ ██      ██   ██
//, ███████ ██████  ██      ███████
//, ██   ██ ██   ██ ██      ██   ██
//, ██   ██ ██   ██  ██████ ██   ██
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, arch* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	dump("name", &obj->name, INDENT);
	dump("primitiveCall", (dummy*)obj->primitiveCall, INDENT);
	dump("CodePlaceSymbol", (dummy*)obj->CodePlaceSymbol, INDENT);
	dump("putComment", (dummy*)obj->putComment, INDENT);
	dump("activate", (dummy*)obj->activate, INDENT);
	dump("cmp", (dummy*)obj->cmp, INDENT);
	dump("jmp", (dummy*)obj->jmp, INDENT);
	dump("jmpPtr", (dummy*)obj->jmpPtr, INDENT);
	dump("jnz", (dummy*)obj->jnz, INDENT);
	dump("jne", (dummy*)obj->jne, INDENT);
	dump("jno", (dummy*)obj->jno, INDENT);
	dump("jns", (dummy*)obj->jns, INDENT);
	dump("js", (dummy*)obj->js, INDENT);
	dump("jz", (dummy*)obj->jz, INDENT);
	dump("je", (dummy*)obj->je, INDENT);
	dump("jo", (dummy*)obj->jo, INDENT);
	dump("jb", (dummy*)obj->jb, INDENT);
	dump("jnae", (dummy*)obj->jnae, INDENT);
	dump("jc", (dummy*)obj->jc, INDENT);
	dump("jnb", (dummy*)obj->jnb, INDENT);
	dump("jae", (dummy*)obj->jae, INDENT);
	dump("jnc", (dummy*)obj->jnc, INDENT);
	dump("jbe", (dummy*)obj->jbe, INDENT);
	dump("jna", (dummy*)obj->jna, INDENT);
	dump("jnbe", (dummy*)obj->jnbe, INDENT);
	dump("ja", (dummy*)obj->ja, INDENT);
	dump("jnge", (dummy*)obj->jnge, INDENT);
	dump("jl", (dummy*)obj->jl, INDENT);
	dump("jge", (dummy*)obj->jge, INDENT);
	dump("jnl", (dummy*)obj->jnl, INDENT);
	dump("jle", (dummy*)obj->jle, INDENT);
	dump("jng", (dummy*)obj->jng, INDENT);
	dump("jnle", (dummy*)obj->jnle, INDENT);
	dump("jg", (dummy*)obj->jg, INDENT);
	dump("jp", (dummy*)obj->jp, INDENT);
	dump("jpe", (dummy*)obj->jpe, INDENT);
	dump("jnp", (dummy*)obj->jnp, INDENT);
	dump("jpo", (dummy*)obj->jpo, INDENT);
	dump("jcxz", (dummy*)obj->jcxz, INDENT);
	dump("jecxz", (dummy*)obj->jecxz, INDENT);
	dump("movRR", (dummy*)obj->movRR, INDENT);
	dump("movUR", (dummy*)obj->movUR, INDENT);
	dump("movUL", (dummy*)obj->movUL, INDENT);
	dump("movLL", (dummy*)obj->movLL, INDENT);
	dump("movLR", (dummy*)obj->movLR, INDENT);
	dump("movRL", (dummy*)obj->movRL, INDENT);
	dump("movVR", (dummy*)obj->movVR, INDENT);
	dump("movRV", (dummy*)obj->movRV, INDENT);
	dump("movVV", (dummy*)obj->movVV, INDENT);
	dump("movLV", (dummy*)obj->movLV, INDENT);
	dump("movVL", (dummy*)obj->movVL, INDENT);
	dump("inc", (dummy*)obj->inc, INDENT);
	dump("dec", (dummy*)obj->dec, INDENT);
	dump("addRR", (dummy*)obj->addRR, INDENT);
	dump("addUR", (dummy*)obj->addUR, INDENT);
	dump("addVVV", (dummy*)obj->addVVV, INDENT);
	dump("mulVVV", (dummy*)obj->mulVVV, INDENT);
	dump("divVVV", (dummy*)obj->divVVV, INDENT);
	dump("modVVV", (dummy*)obj->modVVV, INDENT);
	dump("subRR", (dummy*)obj->subRR, INDENT);
	dump("subUR", (dummy*)obj->subUR, INDENT);
	dump("subVVV", (dummy*)obj->subVVV, INDENT);
	dump("enter", (dummy*)obj->enter, INDENT);
	dump("leave", (dummy*)obj->leave, INDENT);
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
	vlistdump<__register__>("VolatileRegisters", &obj->VolatileRegisters, INDENT);
	vlistdump<__register__>("nonVolatile", &obj->nonVolatile, INDENT);
	dump("setArgStorages", (dummy*)obj->setArgStorages, INDENT);
	dump("moveArguments", (dummy*)obj->moveArguments, INDENT);
	dump("genProlouge", (dummy*)obj->genProlouge, INDENT);
	dump("genEpilouge", (dummy*)obj->genEpilouge, INDENT);
	dump("preCall", (dummy*)obj->preCall, INDENT);
	dump("postCall", (dummy*)obj->postCall, INDENT);
	dump("instrCall", (dummy*)obj->instrCall, INDENT);
	dump("call", (dummy*)obj->call, INDENT);
	dump("integerReturn", &obj->integerReturn, INDENT);
	dump("floatReturn", &obj->floatReturn, INDENT);
	dump("ctorThisRegister", &obj->ctorThisRegister, INDENT);
	GENERIC_OBJ_END
}
//,####################################################################################################################
//,####################################################################################################################
//, ███████ ██    ██ ███    ██  ██████ ████████ ██  ██████  ███    ██     ███████ ████████  ██████  ██████   █████   ██████  ███████
//, ██      ██    ██ ████   ██ ██         ██    ██ ██    ██ ████   ██     ██         ██    ██    ██ ██   ██ ██   ██ ██       ██
//, █████   ██    ██ ██ ██  ██ ██         ██    ██ ██    ██ ██ ██  ██     ███████    ██    ██    ██ ██████  ███████ ██   ███ █████
//, ██      ██    ██ ██  ██ ██ ██         ██    ██ ██    ██ ██  ██ ██          ██    ██    ██    ██ ██   ██ ██   ██ ██    ██ ██
//, ██       ██████  ██   ████  ██████    ██    ██  ██████  ██   ████     ███████    ██     ██████  ██   ██ ██   ██  ██████  ███████
//,####################################################################################################################
//,####################################################################################################################
void dump(std::string name, functionStorage* obj, std::string indent)
{
	GENERIC_NULLPTR
	GENERIC_OBJ_START
	std::cout << INDENT << "DUMP IMPLEMENTATION MISSING" << std::endl;
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
