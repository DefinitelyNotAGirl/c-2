/**
 * Created Date: Tuesday July 25th 2023
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

#include <extint.hxx>
#include <environment.hxx>
#include <string>
#include <vector>

class type;

enum class vaccess {
	_public,
	_protected,
	_private
};

class ImmediateValue {
public:
	bool isSymbol = false;
	uint64_t imm64 = 0;
	std::string symbol = "";
	ImmediateValue(){}
	ImmediateValue(std::string sym)
		:isSymbol(true),symbol(sym){}
	ImmediateValue(uint64_t val)
		:isSymbol(false),imm64(val){}
};

class variable
{
public:
	//. doc info
	std::string desc;
	std::string __declared_file;
    uint64_t __declared_line;
	//. identification
	std::string name;
    std::string symbol;
	//. properties
    type* dataType = nullptr;
    bool doExport = false;
    bool isParameter = false;//only tracked for vstc features
	bool isStatic = false;
	vaccess Access = vaccess::_public;
	bool usedAutoStorage = false;
	//. class properties
    std::vector<variable*> children;//stores members of this variable (ex. gdt.size or var->name)
    variable* parent = nullptr;//only present for child variables
	//. storage
	Architecture storageArch = Architecture::invalid;
	/**
		 @brief  if this->storageArch is storage_IntegerImmediate then this holds a 64-bit integer instead
		 if this->storageArch is storage_member then this holds a 64-bit integer offset into the class
		 if this->storageArch is storage_condition then this holds a currentArch condition code
	*/
	void* storage = nullptr;
};