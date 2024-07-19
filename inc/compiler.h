/**
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday June 3rd 2024 11:44:37 pm
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
#include <SMU.h>
#include <arch.h>
#include <class_variable.h>
#include <class_token.h>
#include <class_line.h>
#include <class_type.h>
#include <class_litop.h>
#include <function.h>
#include <class_scope.h>
#include <stack>
#include <warning.h>
#include <template.h>
#include <ABI.h>
#include <sstream>
#include <system.h>

extern std::vector<castFunction*> castFunctions;
#define currentFile ParserState.File
extern std::vector<std::string> startObjFiles;
extern std::vector<std::string> dependencies;

variable* resolve(token& ft);
std::string getIndent();

void printStacktrace(uint64_t len);

type* getType(std::string name);
variable* getVariable(std::string name);
function* getFunction(std::string name);
function* getFunction(const char* name, std::vector<variable*>& args);
function* getFunction(std::string& name, std::vector<variable*>& args);
function* getFunction(type* returnType, std::string& name, std::vector<variable*>& args);
uint64_t tokenType(std::string& s);
std::string manglePseudoName(std::string& s);
std::string mangleTypeName(std::string& s);

std::string getPrintFunctionExpression(function* f, bool showVariableNames = false);

std::string getNewName();
std::string getNewVariableName();

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

extern type* anyType;
extern type* voidType;
extern type* defaultUnsignedIntegerType;
extern type* defaultSignedIntegerType;
extern type* defaultFloatType;
extern type* defaultCharType;
extern type* defaultWcharType;
extern type* defaultBooleanType;
extern type* defaultPointerType;

extern std::vector<type*> types;
extern std::vector<litop*> litops;
