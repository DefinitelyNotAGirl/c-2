/*
 * Created Date: Sunday September 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday September 17th 2023 7:47:04 am
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
#include <mangling.h>
#include <codegen.h>

void setDefaults()
{
    defaultMangler = getMangler("c+=2");
    if(defaultMangler == nullptr)
    {
        std::cout << "ERROR: could not find default mangler, aborting!" << std::endl;
        exit(-1);
    }
    defaultABI = getABI("cpe2x64");
    if(defaultABI == nullptr)
    {
        std::cout << "ERROR: could not find default ABI, aborting!" << std::endl;
        exit(-1);
    }
    currentArch = getArch("x86-64");
    if(currentArch == nullptr)
    {
        std::cout << "ERROR: could not find default architecture (x86-64), aborting!" << std::endl;
        exit(-1);
    }
    currentArch->activate();
    csys = getSystem("gnu-linux");
    if(csys == nullptr)
    {
        std::cout << "ERROR: could not find default system (gnu-linux), aborting!" << std::endl;
        exit(-1);
    }
    syntax = SYNTAX_INTEL;
}