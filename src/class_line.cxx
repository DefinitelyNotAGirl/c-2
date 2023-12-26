/*
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Monday December 25th 2023 12:32:29 am
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
// 14 - description directive
/**/
// 20 - attribute
// 21 - attribute (function only)
// 22 - attribute (function only primitive)
// 25 - attribute (variable only)
// 26 - attribute (class only)
/**/
// 30 - round brackets
// 31 - square brackets
// 32 - angle brackets
// 33 - curly brackets
/**/
// 40 - colon
// 41 - semicolon
// 42 - comma
/**/
// 50 - template arg type
/**/
// 60 - parameter (gets changed to 10 before return)

extern std::string __reqFileVSTC;

std::list<std::string>* placeHolders = nullptr;
std::string resolvePlaceholder(std::string& s)
{
    if(placeHolders == nullptr)
        return s;
    for(std::string& ph : *placeHolders)
        if(s == ph)
            return ph;
    return s;
}

void* ltobj = nullptr;
bool vstcDisableSend = false;
uint64_t tokenType(std::string& s)
{
    if(s.empty())
        return 0;
    if(s == "__defuint")
        s = defaultUnsignedIntegerType->name;
    else if(s == "__defint")
        s = defaultSignedIntegerType->name;
    else if(s == "__defbool")
        s = defaultBooleanType->name;
    else if(s == "__defchar")
        s = defaultCharType->name;
    else if(s == "__defwchar")
        s = defaultWcharType->name;
    else if(s == "__deffloat")
        s = defaultFloatType->name;
    else if(s == "__defptr")
        s = defaultPointerType->name;
    //brackets
    if(s == "(")return 30;
    else if(s == ")")return 31;
    else if(s == "[")return 32;
    else if(s == "]")return 33;
    else if(s == "<")return 34;
    else if(s == ">")return 35;
    else if(s == "{")return 36;
    else if(s == "}")return 37;
    //misc
    else if(s == ":")return 40;
    else if(s == ";")return 41;
    else if(s == ",")return 42;
    //desc directives
    else if(s == "@desc")return 14;
    else if(s == "@return")return 14;
    else if(s == "@param")return 14;
    else if(s == "@tparam")return 14;
    //directives
    else if(s == "#cum")return 5;
    else if(s == "#include")return 5;
    else if(s == "#include")return 5;
    else if(s == "#define")return 5;
    else if(s == "#pragma")return 5;
    else if(s == "#autodecl")return 5;
    else if(s == "#outcom")return 5;
    else if(s.back() == '"')
    {
        if(s.front() == '"')
        {
            //s = s.substr(1, s.size() - 2);
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
    else if(s.back() == '`')
    {
        if(s.front() == '`')
        {
            //s = s.substr(1, s.size() - 2);
            return 6;
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
    else if(s == "namespace")return 8;
    else if(s == "litop")return 8;
    else if(s == "enum")return 8;
    else if(s == "extends")return 8;
    else if(s == "template")return 8;
    //short ops
    else if(s == "add")return 13;
    else if(s == "sub")return 13;
    else if(s == "mul")return 13;
    else if(s == "div")return 13;
    else if(s == "cast")return 13;
    //attributes
    else if(s == "export")return 20;
    else if(s == "public")return 20;
    else if(s == "protected")return 20;
    else if(s == "private")return 20;
    else if(s == "local")return 20;
    else if(s == "volatile")return 25;
    else if(s == "noalloc")return 25;
    else if(s == "inline")return 21;
    else if(s == "const")return 20;
    else if(s == "constexpr")return 20;
    else if(s == "extern")return 20;
    else if(s == "iteratable")return 20;
    else if(s == "stringifyable")return 20;
    else if(s == "noreturn")return 21;
    else if(s == "typecast")return 21;
    else if(s == "noop")return 21;
    else if(s == "nodoc")return 21;
    else if(s == "deprecated")return 21;
    else if(s == "defaultUnsignedInt")return 26;
    else if(s == "defaultSignedInt")return 26;
    else if(s == "defaultChar")return 26;
    else if(s == "defaultWchar")return 26;
    else if(s == "defaultFloat")return 26;
    else if(s == "defaultBool")return 26;
    else if(s == "defaultPointer")return 26;
    //primitive functions
    else if(s == "primitiveSYSCALL")return 22;
    else if(s == "primitivePRINTCHAR")return 22;
    else if(s == "primitivePRINTSTR")return 22;
    else if(s == "primitiveInterrupt")return 22;
    else if(s == "primitiveArrayIndex")return 22;
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
    //template arg types
    else if(s == "integer")return 50;
    else if(s == "typename")return 50;
    else if(s == "float")return 50;
    else if(s.substr(0,strlen("mangling-")) == "mangling-")return 20;
    else if(s.substr(0,strlen("ABI-")) == "ABI-")return 21;
    else if(s.substr(0,strlen("SYMBOL-")) == "SYMBOL-")return 21;
    else
    {
        //some identifier
        type* gt = getType(s);
        if(gt != nullptr)
        {
            ltobj = gt;
            return 9;//typename
        }
        variable* gv = getVariable(s);
        if(gv != nullptr)
        {
            ltobj = gv;
            if(gv->isParameter)
                return 60;
            return 10;//variable
        }
        function* gf = getFunction(s);
        if(gf != nullptr)
        {
            ltobj = gf;
            return 11;//function
        }

        //new identifier
        return 1;//identifier
    }
}

void sendVstcToken(token& t)
{
    t.type = tokenType(t.text);
    if(options::vstc && currentFile == __reqFileVSTC && !vstcDisableSend)
    {
        std::string ID = "0000";
        std::string mdata = "";
        switch(t.type)
        {
            case(9):
                //typename
                ID="0001";
                mdata = "\x0c"+((type*)ltobj)->__declared_file+"\x0c"+std::to_string(((type*)ltobj)->__declared_line)+"\x0c"+((type*)ltobj)->desc;
                break;
            case(10):
                //variable name
                ID="0002";
                mdata = "\x0c"+((variable*)ltobj)->__declared_file+"\x0c"+std::to_string(((variable*)ltobj)->__declared_line)+"\x0c"+((variable*)ltobj)->dataType->name+"\x0c"+((variable*)ltobj)->desc;
                break;
            case(11):
                //function name
                ID="0003";
                mdata = "\x0c"+((function*)ltobj)->__declared_file+"\x0c"+std::to_string(((function*)ltobj)->__declared_line)+"\x0c"+((function*)ltobj)->returnType->name+"\x0c"+((function*)ltobj)->desc+"\x0c"+((function*)ltobj)->returnDesc;
                break;
            case(60):
                //parameter name
                ID="0004";
                mdata = "\x0c"+((variable*)ltobj)->__declared_file+"\x0c"+std::to_string(((variable*)ltobj)->__declared_line)+"\x0c"+((variable*)ltobj)->dataType->name;
                break;
            default:
                //std::cout << "invalid type: " << t.type << std::endl;
                goto VSTC_NOSEND;
        }
        std::cout << ID << '\x0c' << t.lineNum <<'\x0c'<< t.col <<'\x0c'<< t.text.length() <<'\x0c'<<t.text<<mdata<< '\n';
        VSTC_NOSEND:;
    }
    if(t.type == 60)
        t.type = 10;//change to variable name before returning to compiler
}

token line::nextToken()
{
    //get token text
    token t;
    t.col = this->ccol+this->whitespace;
    this->whitespace = 0;
    uint64_t I = this->tpos;
    while(true)
    {
        switch(this->text[I])
        {
            case(0x00):
                goto textEnd;
            case('\n'):
                if(t.text.size() != 0)
                    goto tokenBreak_space;
                t.col = 0;
                this->tline++;
                break;
            case('\t'):
                if(t.text.size() != 0)
                {
                    //t.col+=tabLength;
                    goto tokenBreak_space;
                }
                t.col+=tabLength;
                break;
            case(' '):
                if(t.text.size() != 0)
                {
                    //t.col++;
                    goto tokenBreak_space;
                }
                t.col++;
                break;
            case('='):
                switch(t.text.back())
                {
                    case('+'):
                    case('-'):
                    case('*'):
                    case('/'):
                    case('%'):
                    case('<'):
                    case('>'):
                    case('&'):
                    case('|'):
                    case('!'):
                        goto __default;
                }
                if(t.text == "operator==")
                    goto __default;
            case('<'):
            case('>'):
                goto skipManglerAndAbiCheck;
            case('-'):
                if(t.text == "ABI")
                    goto __default;
                if(t.text == "mangling")
                    goto __default;
                if(t.text == "SYMBOL")
                    goto __default;
            case('+'):
                skipManglerAndAbiCheck:;
            case('|'):
            case('&'):
                if(t.text.substr(0,strlen("operator")) == "operator" && t.text.back() == this->text[I])
                    goto __default;
                //else
                //    std::cout << "\"" << t.text <<"\" != \"" << "operator" << this->text[I] <<"\"" << std::endl;
            case('/'):
                goto skipPointerTypeCheck;
            case('*'):
                if(getType(t.text) != nullptr)
                    goto __default;
            case('%'):
            case('!'):
                skipPointerTypeCheck:;
                if(t.text == "operator")
                    goto __default;
                if(t.text.substr(0,strlen("ABI-")) == "ABI-")
                    goto __default;
                if(t.text.substr(0,strlen("mangling-")) == "mangling-")
                    goto __default;
                if(t.text.substr(0,strlen("SYMBOL-")) == "SYMBOL-")
                    goto __default;
            case('('):
            case(')'):
            case('{'):
            case('}'):
                goto skipIndexOperator;
            case('['):
                if(t.text == "operator")
                    goto __default;
                goto skipIndexOperator;
            case(']'):
                if(t.text == "operator[")
                    goto __default;
            case(':'):
                skipIndexOperator:;
            case(';'):
            case(','):
                if(t.text.size() == 0)
                    t.text.push_back(this->text[I++]);
                goto tokenBreak;
            case('\''):
            case('`'):
            case('"'):
            {
                char limiter = this->text[I];
                t.text.push_back(this->text[I++]);
                while(true)
                {
                    switch(this->text[I])
                    {
                        case('\\'):
                            t.text.push_back(this->text[I]);
                            t.text.push_back(this->text[++I]);
                            break;
                        case(0x00):
                            goto textEnd;
                        default:
                            if(this->text[I] == limiter)
                            {
                                t.text.push_back(this->text[I]);
                                goto endString;
                            }
                            else
                            {
                                t.text.push_back(this->text[I]);
                            }
                    }
                    I++;
                }
                endString:;
                //std::cout << "string: " << t.text << std::endl;
                break;
            }
            default:
                __default:;
                t.text.push_back(this->text[I]);
                break;
        }
        I++;
    }
    tokenBreak:;
    tokenBreak_space:;
    textEnd:;

    t.type = tokenType(t.text);
    t.Line = this;
    t.lineNum = this->tline;
    this->tpos = I;
    //if(currentFile == __reqFileVSTC)std::cout << "old ccol: " << this->ccol << std::endl;
    this->ccol = t.col+t.text.length();
    //if(currentFile == __reqFileVSTC)std::cout << "token: \"" << t.text << "\"" << std::endl;
    //if(currentFile == __reqFileVSTC)std::cout << "new ccol: " << this->ccol << std::endl;
    if(options::vstc && currentFile == __reqFileVSTC && !vstcDisableSend)
    {
        std::string ID = "0000";
        std::string mdata = "";
        switch(t.type)
        {
            case(9):
                //typename
                ID="0001";
                mdata = "\x0c"+((type*)ltobj)->__declared_file+"\x0c"+std::to_string(((type*)ltobj)->__declared_line)+"\x0c"+((type*)ltobj)->desc;
                break;
            case(10):
                //variable name
                ID="0002";
                mdata = "\x0c"+((variable*)ltobj)->__declared_file+"\x0c"+std::to_string(((variable*)ltobj)->__declared_line)+"\x0c"+((variable*)ltobj)->dataType->name+"\x0c"+((variable*)ltobj)->desc;
                break;
            case(11):
                //function name
                ID="0003";
                mdata = "\x0c"+((function*)ltobj)->__declared_file+"\x0c"+std::to_string(((function*)ltobj)->__declared_line)+"\x0c"+((function*)ltobj)->returnType->name+"\x0c"+((function*)ltobj)->desc+"\x0c"+((function*)ltobj)->returnDesc;
                break;
            case(60):
                //parameter name
                ID="0004";
                mdata = "\x0c"+((variable*)ltobj)->__declared_file+"\x0c"+std::to_string(((variable*)ltobj)->__declared_line)+"\x0c"+((variable*)ltobj)->dataType->name;
                break;
            default:
                //std::cout << "invalid type: " << t.type << std::endl;
                goto VSTC_NOSEND;
        }
        std::cout << ID << '\x0c' << t.lineNum <<'\x0c'<< t.col <<'\x0c'<< t.text.length() <<'\x0c'<<t.text<<mdata<< '\n';
        VSTC_NOSEND:;
    }
    if(t.type == 60)
        t.type = 10;//change to variable name before returning to compiler

    //if(t.type == 6)
    //    t.text = "\""+t.text+"\"";

    if(options::ddebug)
    {
        std::cout << "Token: type: " << t.type << " \"" << t.text <<"\""<< std::endl;
    }

    return t;
}

std::string line::restText()
{
    return this->text.substr(this->tpos,this->text.length());
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

/*
 *  vtables
 */
