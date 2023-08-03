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
#include "master.h"
#include <util.h>

static void genFunctionEpilogueSysV64(std::vector<std::string>& lines, scope* sc)
{
    using enum __register__;

    lines.push_back(getIndent()+sc->name+"epilogue:");

    lines.push_back(getIndent()+"add $"+std::to_string(sc->fstore->stackSize)+", %rsp");

    restoreRegisters();
    popRegSave();

    lines.push_back(getIndent()+"ret");
    //for(std::string& line : lines)
    //{
    //    std::cout << line << std::endl;
    //}
}

void genFunctionEpilogue(std::vector<std::string>& lines, scope* sc)
{
    //generate epilogue
    switch(sc->fstore->ABI)
    {
        case(1):
            genFunctionEpilogueSysV64(lines,sc);
            break;
    }
}