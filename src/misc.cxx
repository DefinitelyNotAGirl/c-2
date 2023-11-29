/*
 * Created Date: Monday July 31st 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday July 31st 2023 1:54:19 pm
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

static uint64_t NewNameCount = 0;

std::string getNewName()
{
    std::string mfile;
    for(char i : currentFile)
    {
        switch(i)
        {
            case('.'):
                mfile+=CPE2_SYMBOL_SCOPE_SEP;
                break;
            case('\\'):
                mfile.push_back('_');
                break;
            case('/'):
                mfile.push_back('_');
                break;
            default:
                mfile.push_back(i);
        }
    }
    return "local"CPE2_SYMBOL_SCOPE_SEP+mfile+CPE2_SYMBOL_SCOPE_SEP+"unnamed"+std::to_string(NewNameCount++);
}

uint64_t getx86MSR(__register__ reg)
{
    using enum __register__;
    switch(reg)
    {
        case(EFER): return 0xc0000080;
    }
    return 0;
};
