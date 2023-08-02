/*
 * Created Date: Wednesday August 2nd 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday August 2nd 2023 4:48:56 am
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

static void genFunctionEpilogueSysV64(scope* sc)
{
    using enum __register__;
    std::vector<std::string> lines;

    lines.push_back(sc->name+"epilogue:");

    lines.push_back("    add $"+std::to_string(sc->fstore->stackSize)+", %rsp");

    if(sc->fstore->registerStatus(rbx) == 1)
    {
        lines.push_back("    pop %rbx");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore rbx as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(rsp) == 1) 
    {
        lines.push_back("    pop %rsp");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore rsp as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(rbp) == 1)
    {
        lines.push_back("    pop %rbp");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore rbp as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r12) == 1)
    {
        lines.push_back("    pop %r12");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore r12 as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r13) == 1)
    {
        lines.push_back("    pop %r13");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore r13 as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r14) == 1) 
    {
        lines.push_back("    pop %r14");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore r14 as mandated by the SystemV amd64 ABI");
    }
    if(sc->fstore->registerStatus(r15) == 1)
    {
        lines.push_back("    pop %r15");
        if(options::asmVerbose == 3)
            lines.back().append(" # restore r15 as mandated by the SystemV amd64 ABI");
    }

    lines.push_back("    ret");
    for(std::string& line : lines)
    {
        std::cout << line << std::endl;
    }
}

void genFunctionEpilogue(scope* sc)
{
    //generate epilogue
    switch(sc->fstore->ABI)
    {
        case(1):
            genFunctionEpilogueSysV64(sc);
            break;
    }
}