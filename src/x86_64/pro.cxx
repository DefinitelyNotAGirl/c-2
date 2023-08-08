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
#include "master.h"
#include <util.h>

static void genFunctionPrologueSysV64(std::vector<std::string>& lines, scope* sc)
{
    using enum __register__;

    lines.push_back(
            "\n# "
            +sc->func->returnType->name
            +" "
            +sc->func->name
            +"(");
    for(type* t : sc->func->parameters)
        lines.back() += (t->name+",");
    if(lines.back().back() == ',')
        lines.back().pop_back();
    lines.back() += (")");

    lines.push_back(sc->func->symbol+":");
    
    sub(sc->fstore->stackSize,__register__::rsp);

    pushRegSave();
    if(sc->fstore->registerStatus(rbx) == 1)
        saveRegister(rbx);
    //if(sc->fstore->registerStatus(rsp) == 1) 
    //    saveRegister(rsp);
    if(sc->fstore->registerStatus(rbp) == 1)
        saveRegister(rbp);
    if(sc->fstore->registerStatus(r12) == 1)
        saveRegister(r12);
    if(sc->fstore->registerStatus(r13) == 1)
        saveRegister(r13);
    if(sc->fstore->registerStatus(r14) == 1) 
        saveRegister(r14);
    if(sc->fstore->registerStatus(r15) == 1)
        saveRegister(r15);

    //for(std::string& line : lines)
    //{
    //    std::cout << line << std::endl;
    //}
}

void genFunctionPrologue(std::vector<std::string>& lines, scope* sc)
{
    //generate prologue
    code = &lines;
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
            genFunctionPrologueSysV64(lines,sc);
            break;
    }
}
