/*
 * Created Date: Thursday August 3rd 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 3rd 2023 6:12:54 pm
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

#include "master.h"

variable* primitiveCall(function* func,std::vector<variable*> args)
{
    variable* ret;
    if(func->primitiveInPlace)
    {
        if(args.size() != 2)
        {
            std::cout << "ERROR: invalid number of arguments to in place primitive function" << std::endl;
        }
        else
        {
            ret = args[0];
        }
    }
    else
    {
        ret = new variable;
    }

    if(!func->primitiveFloat)
    {
        switch(func->op)
        {
            case(primitiveOP::assign):
                if(args.size() != 2)
                    std::cout << "ERROR: invalid number of arguments to in place primitive function" << std::endl;
                else
                    mov(args[1],ret);
                break;
            default:
                std::cout << "blub" << std::endl;
        }
    }
    else
    {
        std::cout << "blub (2)" << std::endl;
    }
    return ret;
}