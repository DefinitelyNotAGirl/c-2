/*
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 25th 2023 12:57:38 am
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

#include <class_line.h>
#include <class_token.h>
#include <common.h>
#include <compiler.h>

//token types:
// 0 - none
// 1 - identifier
// 2 - immediate
// 3 - operator
// 4 - label
// 5 - directive
// 6 - string literal (standard)
// 7 - string literal (widechar)
// 8 - keyword
// 9 - type name
// 10 - variable name
// 11 - function name
// 12 - built in primitive type
// 13 - short OP
/**/
// 20 - attribute
// 21 - attribute (function only)
// 22 - attribute (function only primitive)
// 25 - attribute (variable only)
/**/
// 30 - continue indexing here

uint64_t tokenType(std::string& s)
{
    if(s.empty())
        return 0;
    else if(s == "#include")return 5;
    else if(s == "#define")return 5;
    else if(s.back() == '"')
    {
        if(s.front() == '"')
        {
            s = s.substr(1, s.size() - 2);
            return 6;
        }
        else if(s.front() == 'W' || s.front() == 'L')
        {
            if(s[1] == '"')
            {
                return 7;
                s = s.substr(2, s.size() - 3);
            }
        }
    }
    //storage flags (ex. (rax) or (+28))
    else if(s.front() == '(' && s.back() == ')')return 25;
    //operators
    else if(s[0] == '=')return 3;
    else if(s[0] == '+')return 3;
    else if(s[0] == '-')return 3;
    else if(s[0] == '*')return 3;
    else if(s[0] == '/')return 3;
    else if(s[0] == '%')return 3;
    else if(s[0] == '>')return 3;
    else if(s[0] == '<')return 3;
    else if(s[0] == '|')return 3;
    else if(s[0] == '&')return 3;
    //keywords
    else if(s == "if")return 8;
    else if(s == "else")return 8;
    else if(s == "while")return 8;
    else if(s == "do")return 8;
    else if(s == "for")return 8;
    else if(s == "break")return 8;
    else if(s == "continue")return 8;
    else if(s == "return")return 8;
    else if(s == "switch")return 8;
    else if(s == "case")return 8;
    else if(s == "default")return 8;
    else if(s == "class")return 8;
    else if(s == "litop")return 8;
    else if(s == "enum")return 8;
    else if(s == "extends")return 8;
    //short ops
    else if(s == "add")return 13;
    else if(s == "sub")return 13;
    else if(s == "mul")return 13;
    else if(s == "div")return 13;
    else if(s == "cast")return 13;
    //attributes
    else if(s == "public")return 20;
    else if(s == "protected")return 20;
    else if(s == "private")return 20;
    else if(s == "static")return 20;
    else if(s == "volatile")return 25;
    else if(s == "noalloc")return 25;
    else if(s == "inline")return 21;
    else if(s == "const")return 20;
    else if(s == "extern")return 20;
    else if(s == "noop")return 21;
    //ABIs
    else if(s == "ABI-MICROSOFTx64")return 21;
    else if(s == "ABI-SYSTEMVamd64")return 21;
    //primitive functions
    else if(s == "primitiveInPlace")return 22;
    else if(s == "primitiveFloat")return 22;
    else if(s == "primitiveAdd")return 22;
    else if(s == "primitiveSub")return 22;
    else if(s == "primitiveMul")return 22;
    else if(s == "primitiveDiv")return 22;
    else if(s == "primitiveMod")return 22;
    else if(s == "primitiveEqual")return 22;
    else if(s == "primitiveNotEqual")return 22;
    else if(s == "primitiveGreater")return 22;
    else if(s == "primitiveGreaterEqual")return 22;
    else if(s == "primitiveLess")return 22;
    else if(s == "primitiveLessEqual")return 22;
    else if(s == "primitiveAnd")return 22;
    else if(s == "primitiveXor")return 22;
    else if(s == "primitiveOr")return 22;
    else if(s == "primitiveNot")return 22;
    else if(s == "primitiveInc")return 22;
    else if(s == "primitiveDec")return 22;
    else if(s == "primitiveAssign")return 22;
    //built-in primitive types
    else if(s == "primitiveFloat64")return 12;
    else if(s == "primitiveFloat32")return 12;
    else if(s == "primitive64")return 12;
    else if(s == "primitive32")return 12;
    else if(s == "primitive16")return 12;
    else if(s == "primitive8")return 12;
    else if(s == "primitive0")return 12;
    else
    {
        //some identifier
        if(getType(s) != nullptr)
            return 9;//typename
        if(getVariable(s) != nullptr)
            return 10;//variable
        if(getFunction(s) != nullptr)
            return 11;//function

        //new identifier
        return 1;//identifier
    }
}

token line::nextToken()
{
    token t;
    std::string s;

    t.col = tpos;
    
    for(tpos = tpos;tpos < this->text.size();tpos++)
    {
        switch(this->text[tpos])
        {
            case('"'):
                s += this->text[tpos++];
                while(1)
                {
                    switch(this->text[tpos])
                    {
                        case('"'):
                            s += this->text[tpos++];
                            goto endLoop1;
                        case('\\'):
                            tpos++;
                            switch(this->text[tpos])
                            {
                                case('"'):
                                    tpos++;
                                    s += '"';
                                    break;
                            }
                            break;
                        case(0x00):
                            tpos++;
                            goto endLoop1;
                        default:
                            s += this->text[tpos];
                            tpos++;
                    }
                }
                endLoop2:;
                break;
            case('('):
            case('{'):
            case('['):
                s += this->text[tpos];
                if(s.size() == 1)
                    break;
            case(','):
            case(';'):
                goto endLoop1;
            case(' '):
            case('\n'):
            case('\t'):
                if(s.size() != 0)
                    goto endLoop1;
                break;
            default:
                s += this->text[tpos];
        }
    }
    endLoop1:;
    tpos++;

    t.type = tokenType(s);
    t.Line = this;
    t.text = s;

    if(options::dprintTokens)
    {
        std::cout << "Token: type: " << t.type << " \"" << t.text <<"\""<< std::endl;
    }

    return t;
}

void line::stripTokens(uint64_t n)
{
    for(uint64_t i = 0;i<n;i++)
    {
        this->tpos = 0;
        token t = this->nextToken();
        //std::cout << "stripping token: " << t.text << std::endl;
        //std::cout << "before: \"" << this->text << "\"" << std::endl;
        this->text = this->text.substr(t.text.length()+1,this->text.length());
        //std::cout << "after: \"" << this->text << "\"" << std::endl;
    }
}
