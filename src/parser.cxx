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
#include <sstream>

std::vector<std::string> DataCode;
std::vector<std::string> RoDataCode;
std::vector<std::string> BssCode;
std::vector<std::string> TextCode;
std::vector<std::string> MiscCode;

std::vector<line> getLines(std::string fname);

std::string getIndent()
{
    scope* sc = currentScope->parent;
    std::string ret;
    while(sc != nullptr)
    {
        ret+="    ";
        sc = sc->parent;
    }
    return ret;
}

function* getFunction(std::string name)
{
    return nullptr;
}

std::string getFunctionExpression(function* f)
{
    std::string res = f->returnType->name+" "+f->name + "(";
    for(type* i : f->parameters)
        res+=i->name+",";
    if(res.back() == ',')
        res.pop_back();
    res+=")";
    return res;
}

std::string getFunctionExpression(std::string name, std::vector<variable*> args)
{
    std::string res = name + "(";
    for(variable* i : args)
        res+=i->dataType->name+",";
    if(res.back() == ',')
        res.pop_back();
    res+=")";
    return res;
}

function* getFunction(std::string name, std::vector<variable*> args)
{
    scope* sc = currentScope;
    std::vector<function*> candidates;
    while(sc != nullptr)
    {
        for(function* i : sc->functions)
        {
            if(i->name == name)
            {
                if(i->parameters.size() == args.size())
                {
                    for(uint64_t II = 0;II<i->parameters.size();II++)
                        if(i->parameters[II] != args[II]->dataType)
                            goto notThisFunction;
                        //else
                        //    std::cout <<std::hex<<i->parameters[II]<< " != " << args[II] << std::endl;
                    return i;
                }
                notThisFunction:;
                candidates.push_back(i);
            }
            //else
            //    std::cout << i->name <<" != "<< name << std::endl;
        }
        sc = sc->parent;
    }
    std::cout << "ERROR: cannot find a function to match call \"" << getFunctionExpression(name,args) << "\", candidates are: " << std::endl;
    for(function* candidate : candidates)
        std::cout << "    " << getFunctionExpression(candidate) << std::endl;
    return nullptr;
}

variable* getVariable(std::string name)
{
    scope* sc = currentScope;
    while(sc != nullptr)
    {
        for(variable* i : sc->variables)
            if(i->name == name)
                return i;
        sc = sc->parent;
    }
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
                        std::vector<std::string> lines;
                        genFunctionPrologue(lines,ts);
                        for(std::string& i : ts->func->code)
                            lines.push_back(i);
                        genFunctionEpilogue(lines,ts);
                        for(std::string& i : lines)
                            TextCode.push_back(i);
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
                            bool isPrimitive = false;
                            bool primitiveFloat = false;
                            bool primitiveInPlace = false;
                            primitiveOP op = primitiveOP::invalid;
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
                                    isPrimitive = true;
                                    //primitives
                                    if(attr.text == "primitiveInPlace")
                                        primitiveInPlace = true;
                                    else if(attr.text == "primitiveFloat")
                                        primitiveFloat = true;
                                    else if(attr.text == "primitiveAdd")
                                        op = primitiveOP::add;
                                    else if(attr.text == "primitiveSub")
                                        op = primitiveOP::sub;
                                    else if(attr.text == "primitiveMul")
                                        op = primitiveOP::mul;
                                    else if(attr.text == "primitiveDiv")
                                        op = primitiveOP::div;
                                    else if(attr.text == "primitiveMod")
                                        op = primitiveOP::mod;
                                    else if(attr.text == "primitiveEqual")
                                        op = primitiveOP::equal;
                                    else if(attr.text == "primitiveNotEqual")
                                        op = primitiveOP::NotEqual;
                                    else if(attr.text == "primitiveGreater")
                                        op = primitiveOP::Greater;
                                    else if(attr.text == "primitiveGreaterEqual")
                                        op = primitiveOP::GreaterEqual;
                                    else if(attr.text == "primitiveLess")
                                        op = primitiveOP::Less;
                                    else if(attr.text == "primitiveLessEqual")
                                        op = primitiveOP::LessEqual;
                                    else if(attr.text == "primitiveAnd")
                                        op = primitiveOP::_and;
                                    else if(attr.text == "primitiveXor")
                                        op = primitiveOP::_xor;
                                    else if(attr.text == "primitiveOr")
                                        op = primitiveOP::_or;
                                    else if(attr.text == "primitiveNot")
                                        op = primitiveOP::Not;
                                    else if(attr.text == "primitiveInc")
                                        op = primitiveOP::Inc;
                                    else if(attr.text == "primitiveDec")
                                        op = primitiveOP::Dec;
                                    else if(attr.text == "primitiveAssign")
                                        op = primitiveOP::assign;
                                }
                                else
                                {
                                    error::invalidFunctionAttribute(attr);
                                    goto ERRORENDLINE;
                                }
                            }
                            type* returnType = nullptr;
                            std::vector<type*> paramTypes;
                            std::vector<variable*> arguments;
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
                                        if(arguments.size() != 0)
                                            if(arguments.back()->name == "")
                                                arguments.back()->name = getNewName();

                                        arguments.push_back(new variable);
                                        arguments.back()->dataType = returnType;
                                        if(returnType == nullptr)
                                        {
                                            error::noSuchType(t);
                                            goto ERRORENDLINE;
                                        }
                                        break;
                                    case(1):
                                        if(returnType != nullptr)
                                        {
                                            if(arguments.size() != 0)
                                                arguments.back()->name = t.text;
                                            returnType = nullptr;
                                            break;
                                        }
                                    default:
                                        error::expectedTypename(t);
                                        goto ERRORENDLINE;
                                }
                            }
                            if(arguments.size() != 0)
                                if(arguments.back()->name == "")
                                    arguments.back()->name = getNewName();
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

                            function* func = new function;
                            func->name = name;
                            func->symbol = symbol;
                            func->parameters = paramTypes;
                            func->returnType = returnType;
                            func->fstore = new functionStorage;
                            func->fstore->ABI = ABI;
                            func->isPrimitive = isPrimitive;
                            func->primitiveFloat = primitiveFloat;
                            func->primitiveInPlace = primitiveInPlace;
                            func->op = op;
                            currentScope->functions.push_back(func);

                            if(L.text.back() == ':')
                            {
                                //indent based body
                                scope* sc = new scope;
                                sc->parent = currentScope;
                                sc->name = symbol + "__";
                                sc->leadingSpace = L.leadingSpaces+tabLength;
                                sc->isIndentBased = true;
                                sc->t = scopeType::FUNCTION;
                                sc->fstore = func->fstore;
                                sc->func = func;

                                //declare variables for arguments
                                for(variable* arg : arguments)
                                {
                                    sc->fstore->setArgumentStorage(arg);
                                    if(options::asmVerbose >= 2)
                                    {
                                        std::string comment = "    # "+arg->name+" is stored ";
                                        if(arg->storage == storageType::MEMORY)
                                        {
                                            if(arg->offset < 0)
                                                comment+="at SP";
                                            else
                                                comment+="at SP+";
                                            comment+=std::to_string((int64_t)arg->offset);
                                        }
                                        else if(arg->storage == storageType::REGISTER)
                                        {
                                            comment+="in "+registerNAME(arg->reg);
                                        }
                                        else if(arg->storage == storageType::MEMORY_ABSOLUTE)
                                        {
                                            std::stringstream stream;
                                            stream << std::hex << (int64_t)arg->offset;
                                            comment+="at 0x"+stream.str();
                                        }
                                        sc->func->code.push_back(comment);
                                    }
                                }
                                currentScope = sc;
                                if(options::ddebug)std::cout << "body started" << std::endl;
                                MiscCode.push_back("global "+symbol);
                            }
                            else if(L.text.back() == ';')
                            {
                                //function declaration
                                if(!isPrimitive)
                                    MiscCode.push_back("extern "+symbol);
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
                            std::string symbol = currentScope->name + mangleTypeName(it->name) + "__" + manglePseudoName(name);
                            var->name = name;
                            var->symbol = symbol;
                            if(options::asmVerbose >= 2 && currentScope->t == scopeType::FUNCTION)
                            {
                                std::string comment = getIndent()+"# "+var->name+" is stored ";
                                if(var->storage == storageType::MEMORY)
                                {
                                    if(var->offset < 0)
                                        comment+="at SP";
                                    else
                                        comment+="at SP+";
                                    comment+=std::to_string((int64_t)var->offset);
                                }
                                else if(var->storage == storageType::REGISTER)
                                {
                                    comment+="in "+registerNAME(var->reg);
                                }
                                else if(var->storage == storageType::MEMORY_ABSOLUTE)
                                {
                                    std::stringstream stream;
                                    stream << std::hex << (int64_t)var->offset;
                                    comment+="at 0x"+stream.str();
                                }
                                currentScope->func->code.push_back(comment);
                            }
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
                            currentScope->variables.push_back(var);
                            t = L.nextToken();
                            switch(t.type)
                            {
                                case(3)://operator
                                {
                                    line tmpLine = L;
                                    tmpLine.stripTokens(attribs.size()+1);
                                    std::vector<line> tmpLines;
                                    tmpLines.push_back(tmpLine);
                                    parse(tmpLines);
                                    break;
                                }
                                case(1):
                                    std::cout << "token text: \"" << t.text <<"\""<<std::endl;
                                    break;
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
            case(10):
            {
                //do smth with variable
                std::string name = t.text;
                variable* arg1 = getVariable(t.text);
                t = L.nextToken();
                switch(t.type)
                {
                    case(3):
                        std::string functionName;
                        if(t.text == "=")
                            functionName = "operator=";
                        
                        t = L.nextToken();
                        variable* arg2 = resolve(t);
                        if(arg2 == nullptr)
                        {
                            std::cout << "wtf?" << std::endl;
                        }
                        else
                        {
                            std::vector<variable*> args;
                            args.push_back(arg1);
                            args.push_back(arg2);
                            function* func = getFunction(functionName,args);
                            if(func != nullptr)
                            {
                                if(options::asmVerbose >= 3)
                                    currentScope->func->code.push_back(getIndent()+"# "+L.text);
                                call(func,args);
                            }
                        }
                }
                break;
            }
            case(11):
            {
                //function call
                break;
            }
        }

        ERRORENDLINE:;
        if(++i >= lines.size())
            break;
    }
}