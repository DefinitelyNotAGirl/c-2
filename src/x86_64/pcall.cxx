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
    using enum __register__;
    variable* ret;
    if(func->primitiveInPlace)
    {
        ret = args[0];
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
                //std::cout << "assign!" << std::endl;
                mov(args[1],ret);
                break;
            case(primitiveOP::mul):
                //std::cout << "multiply!" << std::endl;
                if(func->primitiveInPlace)
                {
                    mul(args[0],args[0],args[1]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    mul(ret,args[0],args[1]);
                }
                break;
            case(primitiveOP::div):
                //std::cout << "divide!" << std::endl;
                if(func->primitiveInPlace)
                {
                    div(args[0],args[0],args[1]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    div(ret,args[0],args[1]);
                }
                break;
            case(primitiveOP::mod):
                //std::cout << "modulo!" << std::endl;
                if(func->primitiveInPlace)
                {
                    mod(args[0],args[0],args[1]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    mod(ret,args[0],args[1]);
                }
                break;
            case(primitiveOP::add):
                //std::cout << "add!" << std::endl;
                if(func->primitiveInPlace)
                {
                    add(args[0],args[0],args[1]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    add(ret,args[0],args[1]);
                }
                break;
            case(primitiveOP::sub):
                //std::cout << "sub!" << std::endl;
                if(func->primitiveInPlace)
                {
                    add(args[0],args[0],args[1]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    add(ret,args[0],args[1]);
                }
                break;
            case(primitiveOP::Inc):
                //std::cout << "inc!" << std::endl;
                if(func->primitiveInPlace)
                {
                    inc(args[0]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    mov(args[0],ret);
                    inc(ret);
                }
                break;
            case(primitiveOP::Dec):
                //std::cout << "dec!" << std::endl;
                if(func->primitiveInPlace)
                {
                    dec(args[0]);
                }
                else
                {
                    ret->dataType = args[0]->dataType;
                    ret->name = getNewName();
                    fstore->setStorage(func,ret);
                    mov(args[0],ret);
                    dec(ret);
                }
                break;
            default:
                std::cout << "blub ("<<func->name<<")" << std::endl;
        }
    }
    else
    {
        std::cout << "blub (2)" << std::endl;
    }
    return ret;
}