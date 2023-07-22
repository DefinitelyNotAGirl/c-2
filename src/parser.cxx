/*
 * Created Date: Saturday July 15th 2023
 * Author: Lilith
 * -----
 * Last Modified: Tuesday July 18th 2023 4:48:23 pm
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
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

#include <common.h>
#include <options.h>
#include <compiler.hxx>
#include <util.h>
#include <errors.h>

void pushScope()
{
    scope* ns = (scope*)malloc(sizeof(scope));
    ns->parent = currentScope;
    ns->defines = std::vector<define*>();
    ns->functions = std::vector<function*>();
    ns->variables = std::vector<variable*>();
    ns->types = std::vector<type*>();
    currentScope = ns;
}

void popScope()
{
    scope* oldScope = currentScope;
    currentScope = currentScope->parent;

    free(oldScope);
}

void parse(std::vector<token_t*> tokens)
{
    std::cout << "\033[33m";
    pushScope();
    for(uint64_t c = 0;c<tokens.size();c++)
    {
        token_t* i = tokens[c];
        //std::cout << std::endl << "############"<<std::endl;
        if(options::dprintTokens)
            std::cout << "token: " << i->text << " type: " << tokenToString(i->type) << std::endl;
        //std::cout << "first char: " << i->text[0] << std::endl;
        if(i->text[0] == '#')
        {
            strToLower(i->text);
            //directive
            if(cstrcmp(i->text,(codechar*)"#include"))
            {
                //std::cout << "include" << std::endl;
                //std::cout << "str char 0: " << tokens[c+1]->text[0] << std::endl;
                switch(tokens[c+1]->text[0])
                {
                    case('"'):
                    {
                        if(tokens[c+1]->text[strlen(tokens[c+1]->text)-1] != '"')
                            goto invalidString;
                        goto validString;
                    }
                    case('\''):
                    {
                        if(tokens[c+1]->text[strlen(tokens[c+1]->text)-1] != '\'')
                            goto invalidString;
                        goto validString;
                    }
                    case('<'):
                    {
                        if(tokens[c+1]->text[strlen(tokens[c+1]->text)-1] != '>')
                            goto invalidString;

                        validString:;   
                        {
                            //record string from 1 to strlen-2 ==
                            uint64_t size = (strlen(tokens[c+1]->text)-2)*sizeof(codechar);
                            std::cout << "string size: " << size << std::endl;
                            char* file = (char*)malloc(size+1);
                            file[size] = 0;
                            memcpy(file,tokens[c+1]->text+1,size);
                            std::cout << "including file: " << file << std::endl;
                            std::vector<token_t*> newTokens;
                            token_t* nt = (token_t*)malloc(sizeof(token_t));
                            nt->text = ";";
                            nt->column = 0;
                            nt->line = 0;
                            nt->type = TokenType::TERMINATOR;
                            newTokens.push_back(nt);
                            for(auto t : tokenizeFile(file))
                                newTokens.push_back(t);
                            inject(newTokens,tokens,c+2);
                            c+=1;
                            std::cout << "\033[33m";
                            goto skipInvalidString;
                        }


                        invalidString:;
                        //std::cout << "last char: " << tokens[c+1]->text[strlen(tokens[c+1]->text)-1] << std::endl;
                        //exit(-1);
                        skipInvalidString:;
                    }
                    default:
                        break;
                }
            }
            else if(cstrcmp(i->text,(codechar*)"#define"))
            {
                i = tokens[++c];
                if(!isLatinChar(i->text[0]))
                {
                    error::variableNameLatinChar(c,tokens);
                    std::cout << "\033[33m";
                }
                if(strContains(i->text,'('))
                {
                    //function
                }
                else
                {
                    // standard macro
                    i = tokens[++c];
                    resolveIMM(i->text);
                }
            }
            else
            {
                //std::cout << "directive: " << i->text << std::endl;
            }
        }
        else if(cstrcmp(i->text,(codechar*)"litop"))
        {
            if(c!=0 && tokens[c-1]->type != TokenType::TERMINATOR)
            {
                error::terminatorB4Token(c,tokens);
            }
            else
            {
                codechar* name = tokens[c+1]->text;
                if(cstrcmp(tokens[c+2]->text,(codechar*)"mul"))
                {
                    litopIMM* lit = (litopIMM*)malloc(sizeof(litopIMM));
                    lit->name = name;
                    lit->type = litopType::IMM;
                    lit->op = arithType::MUL;
                    expression* exp = resolveIMM(tokens[c+3]->text);
                    if(exp->type != exprType::IMM)
                    {
                        error::litopValueImmediate(c+3,tokens);
                    }
                    switch(((exprIMM*)exp)->vType)
                    {
                        case((uint64_t)langTypes::u64):
                        case((uint64_t)langTypes::u32):
                        case((uint64_t)langTypes::u16):
                        case((uint64_t)langTypes::u8):
                            lit->rIsSigned = 0;
                            break;
                        case((uint64_t)langTypes::i64):
                        case((uint64_t)langTypes::i32):
                        case((uint64_t)langTypes::i16):
                        case((uint64_t)langTypes::i8):
                            lit->rIsSigned = 1;
                            break;
                        default:
                            lit->rIsSigned = 1;
                    }
                    lit->rightHand = ((exprIMM*)exp)->value;
                    litops.push_back((litop*)lit);
                }
                else if(cstrcmp(tokens[c+2]->text,(codechar*)"add"))
                {
                    litopIMM* lit = (litopIMM*)malloc(sizeof(litopIMM));
                    lit->name = name;
                    lit->type = litopType::IMM;
                    lit->op = arithType::ADD;
                    expression* exp = resolveIMM(tokens[c+3]->text);
                    if(exp->type != exprType::IMM)
                    {
                        error::litopValueImmediate(c+3,tokens);
                    }
                    switch(((exprIMM*)exp)->vType)
                    {
                        case((uint64_t)langTypes::u64):
                        case((uint64_t)langTypes::u32):
                        case((uint64_t)langTypes::u16):
                        case((uint64_t)langTypes::u8):
                            lit->rIsSigned = 0;
                            break;
                        case((uint64_t)langTypes::i64):
                        case((uint64_t)langTypes::i32):
                        case((uint64_t)langTypes::i16):
                        case((uint64_t)langTypes::i8):
                            lit->rIsSigned = 1;
                            break;
                        default:
                            lit->rIsSigned = 1;
                    }
                    lit->rightHand = ((exprIMM*)exp)->value;
                    litops.push_back((litop*)lit);
                }
                else if(cstrcmp(tokens[c+2]->text,(codechar*)"sub"))
                {
                    litopIMM* lit = (litopIMM*)malloc(sizeof(litopIMM));
                    lit->name = name;
                    lit->type = litopType::IMM;
                    lit->op = arithType::SUB;
                    expression* exp = resolveIMM(tokens[c+3]->text);
                    if(exp->type != exprType::IMM)
                    {
                        error::litopValueImmediate(c+3,tokens);
                    }
                    switch(((exprIMM*)exp)->vType)
                    {
                        case((uint64_t)langTypes::u64):
                        case((uint64_t)langTypes::u32):
                        case((uint64_t)langTypes::u16):
                        case((uint64_t)langTypes::u8):
                            lit->rIsSigned = 0;
                            break;
                        case((uint64_t)langTypes::i64):
                        case((uint64_t)langTypes::i32):
                        case((uint64_t)langTypes::i16):
                        case((uint64_t)langTypes::i8):
                            lit->rIsSigned = 1;
                            break;
                        default:
                            lit->rIsSigned = 1;
                    }
                    lit->rightHand = ((exprIMM*)exp)->value;
                    litops.push_back((litop*)lit);
                }
                else if(cstrcmp(tokens[c+2]->text,(codechar*)"div"))
                {
                    litopIMM* lit = (litopIMM*)malloc(sizeof(litopIMM));
                    lit->name = name;
                    lit->type = litopType::IMM;
                    lit->op = arithType::DIV;
                    expression* exp = resolveIMM(tokens[c+3]->text);
                    if(exp->type != exprType::IMM)
                    {
                        error::litopValueImmediate(c+3,tokens);
                    }
                    switch(((exprIMM*)exp)->vType)
                    {
                        case((uint64_t)langTypes::u64):
                        case((uint64_t)langTypes::u32):
                        case((uint64_t)langTypes::u16):
                        case((uint64_t)langTypes::u8):
                            lit->rIsSigned = 0;
                            break;
                        case((uint64_t)langTypes::i64):
                        case((uint64_t)langTypes::i32):
                        case((uint64_t)langTypes::i16):
                        case((uint64_t)langTypes::i8):
                            lit->rIsSigned = 1;
                            break;
                        default:
                            lit->rIsSigned = 1;
                    }
                    lit->rightHand = ((exprIMM*)exp)->value;
                    litops.push_back((litop*)lit);
                }
                else
                    error::invalidLitopOP(c,tokens);
            }
        }
    }
}