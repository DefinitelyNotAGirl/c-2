/*
 * Created Date: Thursday August 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 7:46:56 pm
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

#define constructor __attribute__ ((constructor))

#include <mangling.h>

namespace __mangler__
{

    static void mangleFunction(function* func)
    {
        std::string symbol;
        for(char i : func->name)
        {
            switch(i)
            {
                case('+'):
                    symbol+="ADD";
                    break;
                case('-'):
                    symbol+="SUB";
                    break;
                case('*'):
                    symbol+="MUL";
                    break;
                case('/'):
                    symbol+="DIV";
                    break;
                case('!'):
                    symbol+="NEGATE";
                    break;
                case('<'):
                    symbol+="LESSER";
                    break;
                case('>'):
                    symbol+="GREATER";
                    break;
                case('='):
                    symbol+="EQUAL";
                    break;
                case('|'):
                    symbol+="OR";
                    break;
                case('&'):
                    symbol+="AND";
                    break;
                case('^'):
                    symbol+="EXP";
                    break;
                default:
                    symbol.push_back(i);
            }
        }
        func->symbol = symbol;
    }

    static void mangleVariable(variable* var)
    {
        std::string symbol;
        for(char i : var->name)
        {
            switch(i)
            {
                case('+'):
                    symbol+="ADD";
                    break;
                case('-'):
                    symbol+="SUB";
                    break;
                case('*'):
                    symbol+="MUL";
                    break;
                case('/'):
                    symbol+="DIV";
                    break;
                case('!'):
                    symbol+="NEGATE";
                    break;
                case('<'):
                    symbol+="LESSER";
                    break;
                case('>'):
                    symbol+="GREATER";
                    break;
                case('='):
                    symbol+="EQUAL";
                    break;
                case('|'):
                    symbol+="OR";
                    break;
                case('&'):
                    symbol+="AND";
                    break;
                case('^'):
                    symbol+="EXP";
                    break;
                default:
                    symbol.push_back(i);
            }
        }
        var->symbol = symbol;
    }

    static void mangleType(type* t)
    {
        std::string res;
        for(char i : t->name)
        {
            switch(i)
            {
                case('*'):
                    res+="POINTER";
                    break;
                default:
                    res.push_back(i);
            }
        }
        t->mangledName = res;
    }

    constructor static void init()
    {
        mangler* m = new mangler;
        m->name = "c";
        m->mangleFunction = &mangleFunction;
        m->mangleType = &mangleType;
        m->mangleVariable = &mangleVariable;

        manglers.push_back(m);
    }
}
