/**
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 9:04:51 pm
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

class ABI;

#include <common.h>
#include <options.h>
#include <class_line.h>
#include <class_token.h>
#include <class_type.h>
#include <class_litop.h>
#include <class_variable.h>
#include <storage.h>
#include <function.h>
#include <class_scope.h>
#include <stack>
#include <warning.h>
#include <section.h>
#include <template.h>
#include <arch.h>
#include <ABI.h>
#include <sstream>
#include <system.h>

extern std::vector<std::string> DataCode;
extern std::vector<std::string> RoDataCode;
extern std::vector<std::string> TextCode;
extern std::vector<std::string> BssCode;
extern std::vector<std::string> MiscCode;
extern std::vector<std::string> DebugCode;
extern std::vector<std::string> DebugAbbrevCode;
extern uint64_t debugInfoSize;

extern std::string currentFile;

extern std::vector<std::string> startObjFiles;

extern std::vector<std::string> dependencies;

variable* resolve(token& t);
variable* resolveIMM(char* token);

std::string getIndent();

type* getType(std::string name);
variable* getVariable(std::string name);
function* getFunction(std::string name);
function* getFunction(const char* name, std::vector<variable*>& args);
function* getFunction(std::string& name, std::vector<variable*>& args);
uint64_t tokenType(std::string& s);
std::string manglePseudoName(std::string& s);
std::string mangleTypeName(std::string& s);

std::string getNewName();

ABI* getABI(std::string name);

void resetScope();

void printVariable(variable* v);

std::string intToString(uint64_t n);

extern uint64_t moClassID;
extern uint64_t moFunctionID;
extern uint64_t moVariableID;
extern uint64_t moScopeID;

extern scope* globalScope;
extern scope* currentScope;

extern type* defaultUnsignedIntegerType;
extern type* defaultSignedIntegerType;
extern type* defaultFloatType;
extern type* defaultCharType;
extern type* defaultWcharType;
extern type* defaultBooleanType;
extern type* defaultPointerType;

extern std::vector<type*> types;
extern std::vector<litop*> litops;
