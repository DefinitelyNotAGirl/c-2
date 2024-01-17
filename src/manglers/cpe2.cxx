/*
 * Created Date: Thursday August 17th 2023
 * Author: Lilith
 * -----
 * Last Modified: Thursday August 17th 2023 9:04:51 pm
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
#include <format>

namespace __mangler__
{

    static void mangleFunction(function* func)
    {
        std::string symbol;
        if(currentScope->parent != nullptr)
            symbol = currentScope->name +CPE2_SYMBOL_SCOPE_SEP+ func->returnType->mangledName + "_";
        else
            symbol = func->returnType->mangledName + "_";
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
        for(type* p : func->parameters)
            symbol+="_"+p->mangledName;
        func->symbol = symbol;
    }

    static void mangleVariable(variable* var)
    {
        std::string symbol;
        if(currentScope->parent != nullptr)
            symbol = currentScope->name +CPE2_SYMBOL_SCOPE_SEP+ var->dataType->mangledName + "_";
        else
            symbol = var->dataType->mangledName + "_";
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

    static std::string mangleTarg(std::string in)
    {
        std::string res = "";
        if(in.front() == '"' && in.back() == '"')
        {
            //string
            res+="string_0x";
            for(char i : in.substr(1,in.length()-2))
            {
                res+=std::format("{:x}", (uint64_t)i);
            }
        }
        else if(isDigit(in.front()))
        {
            res = in;
        }
        else
        {
            res = getType(in)->mangledName;
        }
        return res;
    }

    static void mangleType(type* t)
    {
        std::string res;
        for(uint64_t I = 0;I<t->name.length();I++)
        {
            char i = t->name[I];
            switch(i)
            {
                case(0x00):
                    goto textEnd;
                case('*'):
                    res+="P"+std::to_string(t->size);
                    break;
                case('&'):
                    res+="R"+std::to_string(t->size);
                    break;
                case('<'):
                {
                    res+="__templateInstance";
                    i = t->name[++I];
                    uint64_t cbracec = 0;
                    std::string working;
                    while(true)
                    {
                        switch(i)
                        {
                            case(0x00):
                                //error, end of text buffer mid expression
                                std::cout << "error: end of text buffer mid template expression" << std::endl;
                                goto expressionEnded;
                            case('>'):
                                if(cbracec==0)
                                    goto expressionEnded;
                                working.push_back(i);
                                cbracec--;
                                break;
                            case('<'):
                                cbracec++;
                            case(','):
                                if(cbracec==0)
                                {
                                    res+="_"+mangleTarg(working);
                                    working = "";
                                }
                                break;
                            default:
                                working.push_back(i);
                        }
                        i = t->name[++I];
                    }
                    expressionEnded:;
                    if(working != "")
                        res+="_"+mangleTarg(working);
                    I++;
                    break;
                }
                default:
                    res.push_back(i);
            }
        }
        textEnd:;
        t->mangledName = res;
    }

    constructor static void init()
    {
        mangler* m = new mangler;
        m->name = "c+=2";
        m->mangleFunction = &mangleFunction;
        m->mangleType = &mangleType;
        m->mangleVariable = &mangleVariable;

        manglers.push_back(m);
    }
}
