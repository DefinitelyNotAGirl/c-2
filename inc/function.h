/**
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday May 22nd 2024 11:30:22 am
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023-2023 DefinitelyNotAGirl@github
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
#pragma once

struct castFunction;

#include <compiler.h>
#include <stackframe.hxx>
#include <cpustate.hxx>
#include <colors.h>

namespace smu {
	class section;
}
using smu::section;

enum class primitiveOP : uint64_t
{
    invalid = 0,
    add,
    sub,
    mul,
    div,
    mod,
    assign,
    _or,
    _xor,
    _and,
    equal,
    NotEqual,
    Greater,
    GreaterEqual,
    Less,
    LessEqual,
    Not,
    Inc,
    Dec,
    Index,
    Interrupt,
    CPUid,
    PRINTCHAR,
    PRINTSTR,
    SYSCALL
};

/*
    miscData:
        0: is cast function
		1: is async routine
        2-63: reserved MBZ
*/
class function
{
public:
	/*
		. basic
	*/
	std::string name;
	type* returnType;
	variable* returnValue;
	std::vector<type*> parameters;
    std::vector<variable*> vparams;

	/*
		. description
	*/
	std::string desc;
	std::string returnDesc;
	std::string __declared_file;
    uint64_t __declared_line;
	bool isDeprecated = false;
	bool noDoc = false;
	bool doExport = false;

	/*
		. linking information
	*/
	std::string symbol;
	bool isLocal = false;//static

	/*
		. code generation
	*/
	section* code;
	ABI* abi = nullptr;
	bool isPrimitive = false;
    primitiveOP op = primitiveOP::invalid;
    bool primitiveInPlace = false;
    bool primitiveFloat = false;
	bool noReturn = false;
	bool isInline = false;
    bool ignoreCall = false;
	bool isMember = false;

	/*
	 . machine state
	 */
	stackframe stack;
	cpustate cpu;

	/*
		. misc
	*/
    uint64_t miscData = 0;
	void* miscData1; //- this stores a FUNCTION_INLINE* for inline functions

	/**
	 * @brief Get the Function Expression colored using ANSI escape codes
	 * 
	 * @callgraph
	 * @callergraph
	 * 
	 * @return std::string 
	 */
	std::string expression_ansi() {
		std::string res = COLOR_TYPE + this->returnType->name + " "+ COLOR_FUNCTION + this->name +COLOR_RESET+ "(";
		for (variable* i : this->vparams)
		{
			res += COLOR_TYPE + i->dataType->name + " " +COLOR_VAR+ i->name +COLOR_RESET+ ",";
		}
		if (res.back() == ',') res.pop_back();
		res += ")";
		return res;
	}
};

struct castFunction
{
    type* input;
    type* output;
    function* func;
	bool canImplicitCast;
};