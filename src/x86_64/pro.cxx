/*
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday August 2nd 2023 5:12:10 am
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

#include <compiler.h>
#include <util.h>

static void genFunctionPrologueSysV64(scope* sc)
{
    using enum __register__;
    std::vector<std::string> lines;

    lines.push_back(sc->func->symbol+":");

    if(sc->fstore->registerStatus(rbx) == 1)
    {
        lines.push_back("    push %rbx");
        if(options::asmVerbose == 3)
            lines.back().append(" # save rbx as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(rsp) == 1) 
    {
        lines.push_back("    push %rsp");
        if(options::asmVerbose == 3)
            lines.back().append(" # save rsp as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(rbp) == 1)
    {
        lines.push_back("    push %rbp");
        if(options::asmVerbose == 3)
            lines.back().append(" # save rbp as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r12) == 1)
    {
        lines.push_back("    push %r12");
        if(options::asmVerbose == 3)
            lines.back().append(" # save r12 as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r13) == 1)
    {
        lines.push_back("    push %r13");
        if(options::asmVerbose == 3)
            lines.back().append(" # save r13 as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r14) == 1) 
    {
        lines.push_back("    push %r14");
        if(options::asmVerbose == 3)
            lines.back().append(" # save r14 as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r15) == 1)
    {
        lines.push_back("    push %r15");
        if(options::asmVerbose == 3)
            lines.back().append(" # save r15 as mandated by the SystemV amd64 ABI");
    }

    lines.push_back("    sub $"+std::to_string(sc->fstore->stackSize)+", %rsp");

    for(std::string& line : lines)
    {
        std::cout << line << std::endl;
    }
}

void genFunctionPrologue(scope* sc)
{
    //generate prologue
    sc->fstore->stackSize = roundUp(sc->fstore->stackSize, 16);
    if(options::ffunctioninfo)
    {
        std::cout << "##############" << std::endl;
        std::cout << "function: " << sc->func->symbol << std::endl;
        std::cout << "stack size: " << sc->fstore->stackSize << std::endl;
        std::cout << std::endl;
    }

    switch(sc->fstore->ABI)
    {
        case(1):
            genFunctionPrologueSysV64(sc);
            break;
    }
}
