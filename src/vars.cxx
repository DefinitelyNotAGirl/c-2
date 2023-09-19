/*
 * Created Date: Monday July 10th 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 9:04:51 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
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

#include <common.h>
#include <compiler.h>
#include <miscout.h>
#include <mangling.h>

uint64_t moClassID = 0;
uint64_t moFunctionID = 0;
uint64_t moVariableID = 0;
uint64_t moScopeID = 0;

std::vector<mangler*> manglers;
mangler* defaultMangler = nullptr;

arch* currentArch = nullptr;
std::vector<arch*> architectures;

uint64_t ARCH_REGISTER_MASK = 0;

std::vector<format> formats;
std::vector<format*> ClassesOutformats;
std::vector<format*> FunctionsOutformats;
std::vector<format*> VariablesOutformats;
std::vector<format*> ScopesOutformats;
std::list<std::string> sourceFiles;
std::list<std::string> dependencies;
std::vector<type*> types;
std::vector<std::string> includeDirs;
//std::vector<function*> functions;
std::vector<litop*> litops;
scope* globalScope = new scope;
scope* currentScope = globalScope;
#ifdef ASMDEBUG
    #pragma error test
    bool asmDebugComments = true;
#else
    #pragma error test2
    bool asmDebugComments = false;
#endif
uint64_t defaultNumberBase = 10;
uint64_t tabLength = 4;
ABI* defaultABI = nullptr;//default to SystemV amd64 ABI

using enum __register__;
std::vector<__register__> registers = {
                rax,rbx,rcx,rdx,
                rsi,rdi,
                rbp,rsp,
                r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,r13,r14,r15,
                rip,
                cr0,cr1,cr2,cr3,cr4,cr5,cr6,cr7,cr8,cr9,cr10,cr11,cr12,cr13,cr14,cr15,
                EFER,
                DR0,DR1,DR2,DR3,DR6,DR7,
                GDTR,IDTR,LDTR,
                TR,CS,DS,SS,ES,FS,GS,
                xmm0,xmm1,xmm2,xmm3,xmm4,xmm5,xmm6,xmm7,xmm8,xmm9,xmm10,xmm11,xmm12,xmm13,xmm14,xmm15
                };
__register__ StackPointer = rsp;

char c_alert                = 0x07;
char c_backspace            = 0x08;
char c_escape               = 0x1B;
char c_formfeedpagebreak    = 0x0C;
char c_newline              = 0x0A;
char c_cariagereturn        = 0x0D;
char c_horizontaltab        = 0x09;
char c_verticaltab          = 0x0B;

std::string objOut = "";
std::string asmOut = "";

uint64_t POINTER_SIZE = 8;

type* defaultUnsignedIntegerType = nullptr;
type* defaultSignedIntegerType = nullptr;
type* defaultFloatType = nullptr;
type* defaultCharType = nullptr;
type* defaultWcharType = nullptr;
type* defaultBooleanType = nullptr;

std::vector<typeTemplate*> typeTemplates;
std::vector<functionTemplate*> functionTemplates;

std::vector<ABI*> ABIs;

namespace options
{
    //-f******
    bool ffreestanding = false;
    bool fnostdlib = false;
    bool fnolibc = false;
    bool fnoautoinclude = false;
    bool fclasslayout = false;
    bool ffunctioninfo = false;
    bool fvariableinfo = false;
    int fcpl = 3;
    int asmVerbose = 0;
    bool keepComments = false;

    //-m******
    bool mnortti = false;
    bool mnorodata = false;

    //-d******
    bool dprintTokens = false;
    bool ddebug = false;

    //misc
    std::string SysRoot = defaultSysRoot;
    bool MD = false;
    bool C = false;
    bool as = false;
    std::string output = "";

    std::string buildDir = "build";
    std::string docDir = "documentation";
}