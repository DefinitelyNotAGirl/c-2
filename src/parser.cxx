/*
 * Created Date: Tuesday July 25th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 25th 2023 7:30:57 pm
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
#include <options.h>
#include <common.h>
#include <compiler.h>
#include <stdio.h>
#include <execinfo.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <rstack.h>
#include <util.h>
#include <error.h>
#include <rstack.h>
#include <codegen.h>

std::vector<line> getLines(std::string fname);

variable* getVariable(std::string name)
{
    return nullptr;
}

type* getType(std::string name)
{
    for(type* t : types)
        if(t->name == name)
            return t;
    if(name.back() == '*')
    {
        //unknown pointer type, create a new type
        type* t = new type;
        t->name = name;
        t->size = 8;
        types.push_back(t);
        return t;
    }
    return nullptr;
}

revstack<std::string> scopenames;

void parse(std::vector<line> lines)
{
    uint64_t i = 0;
    while(true)
    {
        line& L = lines[i];
        if(L.text.empty())
            break;
        if(options::ddebug)
            std::cout << "line: " << L.text << std::endl;
        token t = L.nextToken();

        bool isPrimitiveFunction = 0;
        bool isFunctionOnly = 0;
        std::list<token> attribs;

        if(options::ddebug)
            std::cout << "indent: " << L.leadingSpaces << " / " << currentScope->leadingSpace << std::endl;
        if((L.leadingSpaces < currentScope->leadingSpace) && currentScope->isIndentBased)
        {
            scope* ts = currentScope;
            while(true)
            {
                if(L.leadingSpaces < ts->leadingSpace)
                {
                    if(options::ddebug)std::cout << "body ended" << std::endl;

                    if(ts->t == scopeType::FUNCTION)
                    {
                        genFunctionPrologue(ts);
                        genFunctionEpilogue(ts);
                    }
                    

                    ts = ts->parent;
                }
                else
                {
                    break;
                }
                if(ts == nullptr)
                {
                    std::cout << "indentation error! (2)" << std::endl;
                    break;
                }
            }
            currentScope = ts;
        }
        else if((L.leadingSpaces > currentScope->leadingSpace) && currentScope->isIndentBased)
        {
            std::cout << "indentation error! (1)" << std::endl;
        }

        switch(t.type)
        {
            case(5)://directive
                if(t.text == "#include")
                {
                    t = L.nextToken();
                    switch(t.type)
                    {
                        case(6)://default width string literal
                            std::cout << "including file: " << t.text << std::endl;
                            parse(getLines(t.text));
                            break;
                    }
                }
                break;
            case(8)://keyword
                if(t.text == "class")
                {
                    t = L.nextToken();
                    switch(t.type)
                    {
                        case(9):{
                            //class redefinition error
                            break;}
                        case(1):{
                            type* ntype = new type;
                            ntype->name = t.text;
                            ntype->size = 0;
                            t = L.nextToken();
                            switch(t.type)
                            {
                                case(1):
                                    if(t.text == ":")
                                        goto collectInheritance;
                                    break;
                                case(8):
                                    if(t.text == "extends")
                                    {
                                        collectInheritance:;
                                        //collect types to inherit from
                                        t = L.nextToken();
                                        int64_t startOffset = 0;
                                        while(t.type == 9 || t.type == 12)
                                        {
                                            if(t.type == 12)
                                            {
                                                if(t.text == "primitive64")     {ntype->size+=8;startOffset+=8;}
                                                else if(t.text == "primitive32"){ntype->size+=4;startOffset+=4;}
                                                else if(t.text == "primitive16"){ntype->size+=2;startOffset+=2;}
                                                else if(t.text == "primitive8") {ntype->size+=1;startOffset+=1;}
                                                else if(t.text == "primitiveFloat64") {ntype->size+=1;startOffset+=8;}
                                                else if(t.text == "primitiveFloat32") {ntype->size+=1;startOffset+=4;}
                                            }
                                            else if(t.type == 9)
                                            {
                                                type* it = getType(t.text);
                                                if(it==nullptr)
                                                {
                                                    error::noSuchType(t);
                                                    delete ntype;
                                                    goto ERRORENDLINE;
                                                }
                                                for(variable m : it->members)
                                                {
                                                    m.offset+=startOffset;
                                                    ntype->members.push_back(m);
                                                }
                                                startOffset+=it->size;
                                            }
                                            t = L.nextToken();
                                        }
                                        if(t.type != 0)
                                        {
                                            error::noSuchType(t);
                                            delete ntype;
                                            goto ERRORENDLINE;
                                        }
                                        ntype->size = startOffset;
                                    }
                                    break;
                            }
                            if(options::fclasslayout)
                            {
                                std::cout << "##############" << std::endl;
                                std::cout << "class: " << ntype->name << std::endl;
                                std::cout << "    size: " << ntype->size << std::endl;
                                for(variable& m : ntype->members)
                                {
                                    if(m.offset > 0)
                                        std::cout << "    (+"<<m.offset<<")";
                                    else
                                        std::cout << "    ("<<m.offset<<")";
                                    std::cout << " " << m.dataType->name << " " << m.name << std::endl;
                                }
                                std::cout << std::endl;
                            }
                            types.push_back(ntype);
                            break;}
                        default:
                            //error unexpected token
                            break;
                    }
                }
                else if(t.text == "litop")
                {
                    t = L.nextToken();
                    switch(t.type)
                    {
                        case(1):
                        {
                            litop* lop = new litop;
                            lop->name = t.text;
                            t = L.nextToken();
                            switch(t.type)
                            {
                                case(13):
                                    if(t.text == "add")
                                        lop->op = shortOP::ADD;
                                    else if(t.text == "sub")
                                        lop->op = shortOP::SUB;
                                    else if(t.text == "mul")
                                        lop->op = shortOP::MUL;
                                    else if(t.text == "div")
                                        lop->op = shortOP::DIV;
                                    else if(t.text == "cast")
                                        lop->op = shortOP::CAST;
                                    
                                    if(lop->op == shortOP::CAST)
                                    {
                                        t = L.nextToken();
                                        switch(t.type)
                                        {
                                            case(9):
                                            {
                                                type* ct = getType(t.text);
                                                if(ct == nullptr)
                                                {
                                                    error::noSuchType(t);
                                                    delete lop;
                                                    goto ERRORENDLINE;
                                                }
                                                break;
                                            }
                                            default:
                                                error::noSuchType(t);
                                                delete lop;
                                                goto ERRORENDLINE;
                                        }
                                    }
                                    else
                                    {
                                        t = L.nextToken();
                                        lop->value = resolve(t)->immediateValue;
                                        litops.push_back(lop);
                                    }
                                    break;
                                default:
                                    error::expectedShortop(t);
                            }
                            break;
                        }
                        default:
                        {
                            error::expectedNewUnique(t);
                        }
                    }
                }
                else if(t.text == "return")
                {
                }
                break;
            case(25):
            case(22):
            case(21):
            case(20):
                collectAttributes:;
                while(t.type >= 20 && t.type < 30)
                {
                    attribs.push_back(t);
                    t = L.nextToken();
                }
                //std::cout << "attributes: ";
                //for(token& a : attribs)
                //    std::cout << a.text << " ";
                //std::cout << std::endl;
            case(9):
                switch(t.type)
                {
                    case(9):{
                        //function or variable declaration
                        type* it = getType(t.text);
                        if(it==nullptr)
                        {
                            error::noSuchType(t);
                            goto ERRORENDLINE;
                        }
                        t = L.nextToken();
                        std::string name = t.text;
                        bool isFunction = false;
                        if(name.back() == '(')
                        {
                            name.pop_back();
                            isFunction = true;
                            //function
                        }
                        else
                        {
                            //check for next non whitespace char
                            uint64_t start = t.col+name.length();
                            for(uint64_t i = start; i < L.text.length(); i++)
                            {
                                switch(L.text[i])
                                {
                                    case(' '):
                                    case('\t'):
                                    case('\n'):
                                        break;
                                    case('('):
                                        //function
                                        isFunction = true;
                                    default:
                                        goto break_1;
                                }
                            }
                            break_1:;
                        }
                        if(isFunction)
                        {
                            bool isStatic = false;
                            bool isInline = false;
                            bool isExtern = false;
                            bool isConst = false;
                            bool isNoop = false;
                            uint8_t access = 0;
                            uint8_t ABI = 0;
                            for(token& attr : attribs)
                            {
                                if(attr.text == "static")
                                    isStatic = true;
                                else if(attr.text == "public")
                                    access = 0;
                                else if(attr.text == "protected")
                                    access = 1;
                                else if(attr.text == "private")
                                    access = 2;
                                else if(attr.text == "inline")
                                    isInline = true;
                                else if(attr.text == "const")
                                    isConst = true;
                                else if(attr.text == "extern")
                                    isExtern = true;
                                else if(attr.text == "noop")
                                    isNoop = true;
                                else if(attr.text.substr(0, 4) == "ABI-")
                                {
                                    std::string abiName = attr.text.substr(4);
                                    if(abiName == "MICROSOFTx64")
                                        ABI = 2;
                                    else if(abiName == "SYSTEMVamd64")
                                        ABI = 1;
                                }
                                else if(attr.text.substr(0,strlen("primitive")) == "primitive")
                                {
                                    //primitives
                                }
                                else
                                {
                                    error::invalidFunctionAttribute(attr);
                                    goto ERRORENDLINE;
                                }
                            }
                            type* returnType = nullptr;
                            std::vector<type*> paramTypes;
                            bool hasMoreParams = true;
                            while(hasMoreParams)
                            {
                                t = L.nextToken();
                                if(t.text.front() == '(')
                                {
                                    t.text = t.text.substr(1);
                                    t.type = tokenType(t.text);
                                }
                                if(t.text.back() == ':')
                                {
                                    t.text.pop_back();
                                    t.type = tokenType(t.text);
                                    hasMoreParams = false;
                                }
                                if(t.text.back() == ')')
                                {
                                    t.text.pop_back();
                                    t.type = tokenType(t.text);
                                    hasMoreParams = false;
                                }
                                switch(t.type)
                                {
                                    case(9):
                                        returnType = getType(t.text);
                                        paramTypes.push_back(returnType);
                                        if(returnType == nullptr)
                                        {
                                            error::noSuchType(t);
                                            goto ERRORENDLINE;
                                        }
                                        break;
                                    case(1):
                                        if(returnType != nullptr)
                                        {
                                            returnType = nullptr;
                                            break;
                                        }
                                    default:
                                        error::expectedTypename(t);
                                        goto ERRORENDLINE;
                                }
                            }
                            returnType = it;

                            if(ABI == 0)
                                ABI = defaultABI;
                            std::string symbol = currentScope->name + mangleTypeName(returnType->name) + "_" + manglePseudoName(name);
                            for(type* p : paramTypes)
                                symbol+="_"+mangleTypeName(p->name);

                            if(options::ffunctioninfo)
                            {
                                std::cout << "##############" << std::endl;
                                std::cout << "function: " << name << std::endl;
                                std::cout << "symbol: " << symbol << std::endl;
                                std::cout << "ABI: ";
                                switch(ABI)
                                {
                                    case(1):
                                        std::cout << "SystemV amd64" << std::endl;
                                        break;
                                    case(2):
                                        std::cout << "Microsoft x64" << std::endl;
                                        break;
                                    default:
                                        std::cout << "Unknown" << std::endl;
                                }
                                std::cout << "return type: " << returnType->name << std::endl;
                                if(paramTypes.size() > 0)
                                {
                                    std::cout << "parameters: " << std::endl;
                                    for(type* p : paramTypes)
                                        std::cout << "        " << p->name << std::endl;
                                }
                                std::cout << std::endl;
                            }

                            if(L.text.back() == ':')
                            {
                                //indent based body
                                scope* sc = new scope;
                                sc->parent = currentScope;
                                sc->name = symbol + "__";
                                sc->leadingSpace = L.leadingSpaces+tabLength;
                                sc->isIndentBased = true;
                                sc->t = scopeType::FUNCTION;
                                sc->fstore = new functionStorage;
                                sc->fstore->ABI = ABI;

                                function* func = new function;
                                func->name = name;
                                func->symbol = symbol;
                                func->parameters = paramTypes;
                                func->returnType = returnType;

                                sc->func = func;
                                currentScope = sc;
                                if(options::ddebug)std::cout << "body started" << std::endl;
                            }
                        }
                        else
                        {
                            variable* var = new variable;
                            var->dataType = it;
                            bool isStatic = false;
                            bool isExtern = false;
                            bool isConst = false;
                            bool isVolatile = false;
                            bool noalloc = false;
                            uint8_t access = 0;
                            for(token& attr : attribs)
                            {
                                if(attr.text[0] == '(' && attr.text.back() == ')')
                                {
                                    //storage attribute
                                    if(isdigit(attr.text[1]))
                                    {
                                        //absolute memory address
                                        token blub = t;
                                        blub.text = attr.text.substr(1, attr.text.size()-2);
                                        uint64_t addr = resolve(blub)->immediateValue;
                                        var->storage = storageType::MEMORY_ABSOLUTE;
                                        var->offset = addr;
                                    }
                                    else if(attr.text[1] == '+' || attr.text[1] == '-')
                                    {
                                        //address relative to SP
                                        token blub = t;
                                        blub.text = attr.text.substr(1, attr.text.size()-2);
                                        uint64_t offset = resolve(blub)->immediateValue;
                                        var->storage = storageType::MEMORY;
                                        var->offset = offset;
                                    }
                                    else
                                    {
                                        //register
                                        std::string rname = attr.text.substr(1, attr.text.size()-2);
                                        __register__ reg = registerID(rname);
                                        if(reg == __register__::invalid)
                                            std::cout << "invalid register name: " << rname << std::endl;
                                        uint64_t rsize = 0x0000FF0000 & (uint64_t)reg;
                                        if(rsize < it->size)
                                        {
                                            std::cout   << "ERROR: register \"" << rname 
                                                        << "\" is too small to fit variable of type \"" 
                                                        << it->name << "\"!" 
                                                        << std::endl;
                                        }
                                        
                                        var->storage = storageType::REGISTER;
                                        var->reg = reg;
                                    }
                                }
                                else if(attr.text == "static")
                                    isStatic = true;
                                else if(attr.text == "public")
                                    access = 0;
                                else if(attr.text == "protected")
                                    access = 1;
                                else if(attr.text == "private")
                                    access = 2;
                                else if(attr.text == "const")
                                    isConst = true;
                                else if(attr.text == "extern")
                                    isExtern = true;
                                else if(attr.text == "noalloc")
                                    noalloc = true;
                                else if(attr.text == "volatile")
                                    isVolatile = true;
                                else
                                {
                                    error::invalidVariableAttribute(attr);
                                    goto ERRORENDLINE;
                                }
                            }
                            if(var->storage == storageType::INVALID)
                            {
                                //auto storage
                                if(currentScope->t == scopeType::FUNCTION)
                                    currentScope->fstore->setStorage(var);
                            }
                            std::string symbol = currentScope->name + mangleTypeName(it->name) + "_" + manglePseudoName(name);
                            if(options::fvariableinfo)
                            {
                                std::cout << "##############" << std::endl;
                                std::cout << "variable: " << name << std::endl;
                                std::cout << "symbol: " << symbol << std::endl;
                                std::cout << "type: " << var->dataType->name << std::endl;
                                std::cout << "storage: ";
                                if(currentScope->t == scopeType::FUNCTION)
                                {
                                    if(var->storage == storageType::MEMORY)
                                    {
                                        if(var->offset < 0)
                                            std::cout << "SP";
                                        else
                                            std::cout << "SP+";
                                        std::cout <<std::dec<< (int64_t)var->offset;
                                    }
                                    else if(var->storage == storageType::REGISTER)
                                    {
                                        std::cout << registerNAME(var->reg);
                                    }
                                    else if(var->storage == storageType::MEMORY_ABSOLUTE)
                                    {
                                        std::cout << "0x" << std::hex << var->offset;
                                    }
                                }
                                else
                                {
                                    if(var->storage == storageType::REGISTER)
                                    {
                                        std::cout << registerNAME(var->reg);
                                    }
                                    else if(var->storage == storageType::MEMORY_ABSOLUTE)
                                    {
                                        std::cout << "0x" << std::hex << var->offset;
                                    }
                                }
                                std::cout << std::endl;
                                std::cout << std::endl;
                            }
                        }
                        
                        break;
                    }
                    case(10):
                    {
                        //do smth with variable

                        break;
                    }
                    case(11):
                    {
                        //function call
                        break;
                    }
                }
            break;
        }

        ERRORENDLINE:;
        if(i++ >= lines.size())
            break;
    }
}