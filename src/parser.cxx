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
    return nullptr;
}

void parse(std::vector<line> lines)
{
    uint64_t i = 0;
    while(true)
    {
        line& L = lines[i];
        if(L.text.empty())
            break;
        if(options::dprintTokens)
            std::cout << "line: " << L.text << std::endl;
        token t = L.nextToken();

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
                                            }
                                            else if(t.type == 9)
                                            {
                                                type* it = getType(t.text);
                                                if(it==nullptr)
                                                {
                                                    std::cout << "\033[31mERROR:\033[0m \"" << t.text << "\" does not name a type" << std::endl;
                                                    std::string lineNumStr = std::to_string(L.lineNum);
                                                    std::cout << "at line: " << lineNumStr << " in file: "<<L.file<<": "<<L.text<<  std::endl;
                                                    uint64_t arrowPos = t.col+lineNumStr.size()+10+11+2+L.file.size()-2;
                                                    for(uint64_t i = 0; i < arrowPos; i++)
                                                        std::cout << ' ';
                                                    std::cout << "\033[31m^\033[0m" << std::endl;
                                                    delete ntype;
                                                    goto ERRORENDLINE;
                                                }
                                                for(member m : it->members)
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
                                            std::cout << "\033[31mERROR:\033[0m \"" << t.text << "\" does not name a type" << std::endl;
                                            std::string lineNumStr = std::to_string(L.lineNum);
                                            std::cout << "at line: " << lineNumStr << " in file: "<<L.file<<": "<<L.text<<  std::endl;
                                            uint64_t arrowPos = t.col+lineNumStr.size()+10+11+2+L.file.size()-2;
                                            for(uint64_t i = 0; i < arrowPos; i++)
                                                std::cout << ' ';
                                            std::cout << "\033[31m^\033[0m" << std::endl;
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
                                for(member& m : ntype->members)
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
                }
                else if(t.text == "return")
                {
                }
                break;
        }

        /*
        //integers
        uint64_t inherited_classes = 0;

        //data structures
        revstack<token> TTS;
        type* ntype = nullptr;
        type* dataType = nullptr;

        while(t.type)
        {
            std::cout << "token: " << t.text << std::endl;

            switch(t.type)
            {
                case(6)://default width string literal
                    if(TTS[0].text == "#include")
                    {
                        std::cout << "including file: " << t.text << std::endl;
                        parse(getLines(t.text));
                    }
                    break;
                case(9):
                    if(TTS[0].text == "class")
                    {
                        std::cout << "ERROR: class \"" << t.text << "\" already exists!" << std::endl;
                    }
                    break;
                case(1):
                    if(TTS.size() > 0)
                    {
                        if(TTS[0].text == "class")
                        {
                            ntype = new type;
                            ntype->name = t.text;
                            std::cout << "new class: " << t.text << std::endl;
                        }
                        else if(TTS.size() > 1 && (TTS[1].text == "class" && t.text == ":"))
                        {
                            std::cout << "fetching inheritance..." << std::endl;
                        }
                        goto case1TokenEnd;
                    }
                    {
                        dataType = getType(t.text);
                        variable* var = getVariable(t.text);
                        if(dataType == nullptr && var == nullptr)
                        {
                            //no such type error
                            std::cout << "\033[31mERROR:\033[0m \"" << t.text << "\" does not name a type or variable!" << std::endl;
                            std::string lineNumStr = std::to_string(L.lineNum);
                            std::cout << "at line: " << lineNumStr << " in file: "<<L.file<<": "<<L.text<<  std::endl;
                            uint64_t arrowPos = t.col+lineNumStr.size()+10+11+2+L.file.size()-2;
                            for(uint64_t i = 0; i < arrowPos; i++)
                                std::cout << ' ';
                            std::cout << "\033[31m^\033[0m" << std::endl;
                            goto ERRORENDLINE;
                        }
                    }
                    case1TokenEnd:;
                    break;
                case(12):
                    if(TTS.size() > 1)
                    {
                        if(TTS[2+inherited_classes].text == "class")
                        {
                            std::cout << "inheriting class: " << t.text << std::endl;
                            inherited_classes++;
                            if(t.text == "primitive64")     ntype->size+=8;
                            else if(t.text == "primitive32")ntype->size+=4;
                            else if(t.text == "primitive16")ntype->size+=2;
                            else if(t.text == "primitive8") ntype->size+=1;
                        }
                    }
                    break;
            }

            TTS.push(t);

            if(ntype!=nullptr)
                types.push_back(ntype);

            //epilogue
            t = L.nextToken();
        }
        */

        ERRORENDLINE:;
        if(i++ >= lines.size())
            break;
    }
}