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
    scope* ns = new scope;
    ns->parent = currentScope;
    //ns->defines = std::vector<define*>::vector();
    //ns->variables = std::vector<variable*>::vector();
    currentScope = ns;
}

void popScope()
{
    scope* oldScope = currentScope;
    currentScope = currentScope->parent;

    free(oldScope);
}

TokenType lastNWSTokenType(uint64_t tPOS, std::vector<token_t*>& tokens)
{
    if(tPOS == 0)
        return TokenType::TERMINATOR;
    for(uint64_t i = tPOS-1; i > 0;i--)
    {
        if(tokens[i]->type != TokenType::WHITESPACE)
            return tokens[i]->type;
    }
    return TokenType::TERMINATOR;
}

variable* getVariable(codechar* name)
{
    scope* sc = currentScope;
    gVarBegin:;
    for(variable* var : sc->variables)
        if(cstrcmp(var->name, name))
            return var;

    if(sc->parent!=nullptr)
    {
        sc = sc->parent;
        goto gVarBegin;
    }

    return nullptr;
}

type* getType(codechar* name)
{
    for(type* t : types)
        if(cstrcmp(t->name, name))
            return t;
        else
            std::cout << t->name << " != " << name << std::endl;
    return nullptr;
}

uint64_t tokensLeft(uint64_t tPOS, std::vector<token_t*>& tokens)
{
    return tokens.size() - tPOS - 1;
}

void printAttr(uint64_t attr)
{
    std::cout << "attributes: " << std::endl;
    if(EXPR_GETBIT_00(attr))
        std::cout << "    " << "macro" << std::endl;
    if(EXPR_GETBIT_01(attr))
        std::cout << "    " << "const" << std::endl;
    if(EXPR_GETBIT_02(attr))
        std::cout << "    " << "static" << std::endl;
    if(EXPR_GETBIT_03(attr))
        std::cout << "    " << "extern" << std::endl;
    if(EXPR_GETBIT_04(attr))
        std::cout << "    " << "volatile" << std::endl;
    if(EXPR_GETBIT_05(attr))
        std::cout << "    " << "constexpr" << std::endl;
    if(EXPR_GETBIT_06(attr))
        std::cout << "    " << "primitive" << std::endl;
    if(EXPR_GETBIT_07(attr))
        std::cout << "    " << "primitive-in-place" << std::endl;
    if(EXPR_GETBIT_06(attr))
    {
        std::cout << "        " <<std::hex<< (uint64_t)(*((uint8_t*)((&attr)+1))) << std::endl;
        std::cout << "        " <<std::hex<< (uint64_t)(*((uint8_t*)((&attr)+2))) << std::endl;
    }
}

void parse(std::vector<token_t*> tokens)
{
    std::cout << "\033[33m";
    pushScope();
    uint64_t attr = 0;
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
            if(lastNWSTokenType(c,tokens) != TokenType::TERMINATOR)
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
        else if(cstrcmp(i->text,(codechar*)"class"))
        {
             
            if(tokensLeft(c,tokens) > 0)
            {
                codechar* name = tokens[c+1]->text;
                type* nt = (type*)calloc(1,sizeof(type));
                nt->name = name;
                if(cstrcmp(tokens[c+2]->text,(codechar*)"{"))
                {
                    //open class body
                }
                else if(cstrcmp(tokens[c+2]->text,(codechar*)":"))
                {
                    //inhertiance
                    for(uint64_t I = 3;I<tokensLeft(c,tokens);I=I)
                    {
                        if(tokensLeft(c+I,tokens) > 1)
                        {
                            uint64_t inheritanceType = 0;
                            codechar* superClass = nullptr;
                            if(tokens[c+I+1]->type == TokenType::TERMINATOR)
                            {
                                inheritanceType = 3;//private inheritance (default)
                                superClass = tokens[c+I]->text;
                                c+=1;
                            }
                            else if(tokensLeft(c+I,tokens) > 2)
                            {
                                if(tokens[c+I+2]->type == TokenType::TERMINATOR)
                                {
                                    if(cstrcmp(tokens[c+I]->text,(codechar*)"public"))
                                        inheritanceType = 1;//public inheritance
                                    else if(cstrcmp(tokens[c+I]->text,(codechar*)"protected"))
                                        inheritanceType = 2;//protected inheritance
                                    else if(cstrcmp(tokens[c+I]->text,(codechar*)"private"))
                                        inheritanceType = 3;//private inheritance
                                    superClass = tokens[c+I+1]->text;
                                    c+=2;
                                }
                            }
                            if(options::fclasslayout)
                            {
                                std::cout << "inheritanceType: " << inheritanceType << std::endl;
                                std::cout << "superClass: " << superClass << std::endl;
                            }
                            //get properties from superclass
                            if(cstrcmp(superClass,(codechar*)"primitive64"))
                                nt->size+=8;
                            else if(cstrcmp(superClass,(codechar*)"primitive32"))
                                nt->size+=4;
                            else if(cstrcmp(superClass,(codechar*)"primitive16"))
                                nt->size+=2;
                            else if(cstrcmp(superClass,(codechar*)"primitive8"))
                                nt->size+=1;

                            if(tokens[c+I]->type == TokenType::TERMINATOR)
                            {
                                if(cstrcmp(tokens[c+I]->text,(codechar*)";"))
                                    break;
                            }
                        }
                    }
                    c+=3;
                }
                else if(tokens[c+2]->text == (codechar*)";")
                {
                    //blank class declaration
                }

                if(options::fclasslayout)
                {
                    std::cout << "class name: " << nt->name << std::endl;
                    std::cout << "class size: " << nt->size << std::endl;
                }
                types.push_back(nt);
            }
        }
        else if(cstrcmp(i->text,(codechar*)"typedef"))
        {
            if(lastNWSTokenType(c,tokens) != TokenType::TERMINATOR)
            {
                error::terminatorB4Token(c,tokens);
            }
            if(tokensLeft(c,tokens) > 0)
            {
            }
        }
        //8 bits
        else if(cstrcmp(i->text,(codechar*)"primitiveEqual_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x01;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotEqual_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x02;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreater_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x03;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreaterEqual_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x04;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLess_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x05;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLessEqual_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x06;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotZero_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x07;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveEqualZero_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x08;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftLeft_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x09;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftRight_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0A;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNegate_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0B;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveInc_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x15;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDec_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x16;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_A_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x00;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_8_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x00;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_8_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x01;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_8_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x02;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_8_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x03;
        }//16 bits
        else if(cstrcmp(i->text,(codechar*)"primitiveEqual_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x01;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotEqual_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x02;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreater_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x03;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreaterEqual_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x04;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLess_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x05;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLessEqual_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x06;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotZero_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x07;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveEqualZero_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x08;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftLeft_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x09;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftRight_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0A;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNegate_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0B;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveInc_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x15;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDec_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x16;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_A_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x11;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_16_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x10;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_16_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x11;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_16_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x12;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_16_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x13;
        }//32 bits
        else if(cstrcmp(i->text,(codechar*)"primitiveEqual_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x01;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotEqual_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x02;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreater_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x03;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreaterEqual_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x04;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLess_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x05;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLessEqual_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x06;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotZero_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x07;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveEqualZero_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x08;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftLeft_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x09;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftRight_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0A;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNegate_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0B;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveInc_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x15;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDec_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x16;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_A_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x22;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_32_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x20;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_32_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x21;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_32_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x22;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_32_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x23;
        }//64 bits
        else if(cstrcmp(i->text,(codechar*)"primitiveEqual_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x01;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotEqual_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x02;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreater_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x03;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveGreaterEqual_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x04;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLess_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x05;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveLessEqual_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x06;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNotZero_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x07;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveEqualZero_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x08;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftLeft_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x09;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveShiftRight_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0A;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveNegate_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0B;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveInc_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x15;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveDec_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x16;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAdd_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0C;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveSub_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0D;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMul_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0E;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveDiv_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x0F;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveOr_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x10;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAnd_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x11;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveXor_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x12;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveMod_A_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x13;
            *((uint8_t*)((&attr)+2)) &= 0x33;
            SETBIT_07(attr);
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_64_8")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x30;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_64_16")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x31;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_64_32")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x32;
        }else if(cstrcmp(i->text,(codechar*)"primitiveAssign_64_64")){
            SETBIT_06(attr);
            *((uint8_t*)((&attr)+1)) &= 0x14;
            *((uint8_t*)((&attr)+2)) &= 0x33;
        }
        else if(cstrcmp(i->text,(codechar*)"static"))
        {
            SETBIT_02(attr);
        }
        else if(cstrcmp(i->text,(codechar*)"const"))
        {
            SETBIT_01(attr);
        }
        else if(cstrcmp(i->text,(codechar*)"constexpr"))
        {
            SETBIT_05(attr);
        }
        else if(cstrcmp(i->text,(codechar*)"extern"))
        {
            SETBIT_03(attr);
        }
        else if(cstrcmp(i->text,(codechar*)"volatile"))
        {
            SETBIT_04(attr);
        }
        else if(i->type == TokenType::TERMINATOR){}
        else
        {
            //not a keyword
            if(tokensLeft(c,tokens) > 2)
            {
                if(cstrcmp(tokens[c+2]->text,(codechar*)"("))
                {
                    std::cout << "function detected" << std::endl;
                    printAttr(attr);
                    attr = 0;

                    std::cout << "c: " <<std::dec << c << std::endl;
                    std::cout << "tokens 0: " << tokens[c+0]->text << std::endl;
                    std::cout << "tokens 1: " << tokens[c+1]->text << std::endl;
                    std::cout << "tokens 2: " << tokens[c+2]->text << std::endl;
                    std::cout << "tokens 3: " << tokens[c+3]->text << std::endl;
                    std::cout << "tokens 4: " << tokens[c+4]->text << std::endl;

                    pushScope();

                    type* ftype = getType(tokens[c]->text);
                    if(ftype == nullptr)
                    {
                        error::noSuchType(c,tokens);
                        exit(-1);
                    }

                    std::string name = ftype->name;
                    name += "_";
                    name += tokens[c+1]->text;

                    while(1)
                    {
                        if(cstrcmp(tokens[c+2]->text,(codechar*)")"))
                        {
                            //std::cout << "c: " << c << std::endl;
                            //std::cout << "tokens 0: " << tokens[c+0]->text << std::endl;
                            //std::cout << "tokens 1: " << tokens[c+1]->text << std::endl;
                            //std::cout << "tokens 2: " << tokens[c+2]->text << std::endl;
                            //std::cout << "tokens 3: " << tokens[c+3]->text << std::endl;
                            //std::cout << "tokens 4: " << tokens[c+4]->text << std::endl;
                            //std::cout << "break" << std::endl;
                            break;
                        }
                        else if(cstrcmp(tokens[c+4]->text,(codechar*)",") || cstrcmp(tokens[c+4]->text,(codechar*)")"))
                        {
                            //typename-only argument
                            //std::cout << "c: " << c << std::endl;
                            //std::cout << "tokens 0: " << tokens[c+0]->text << std::endl;
                            //std::cout << "tokens 1: " << tokens[c+1]->text << std::endl;
                            //std::cout << "tokens 2: " << tokens[c+2]->text << std::endl;
                            //std::cout << "tokens 3: " << tokens[c+3]->text << std::endl;
                            //std::cout << "tokens 4: " << tokens[c+4]->text << std::endl;
                            type* atype = getType(tokens[c+3]->text);
                            if(atype == nullptr)
                            {
                                error::noSuchType(c+2,tokens);
                            }
                            name+="_";
                            name+=atype->name;
                            c+=2;
                        }
                        else if(cstrcmp(tokens[c+5]->text,(codechar*)",") || cstrcmp(tokens[c+5]->text,(codechar*)")"))
                        {
                            //complete argument
                            //std::cout << "c: " << c << std::endl;
                            //std::cout << "tokens 0: " << tokens[c+0]->text << std::endl;
                            //std::cout << "tokens 1: " << tokens[c+1]->text << std::endl;
                            //std::cout << "tokens 2: " << tokens[c+2]->text << std::endl;
                            //std::cout << "tokens 3: " << tokens[c+3]->text << std::endl;
                            //std::cout << "tokens 4: " << tokens[c+4]->text << std::endl;
                            type* atype = getType(tokens[c+3]->text);
                            if(atype == nullptr)
                            {
                                error::noSuchType(c+3,tokens);
                            }
                            variable* aVar = getVariable(tokens[c+4]->text);
                            if(aVar != nullptr)
                            {
                                //redeclaration of variable
                            }
                            else
                            {
                                //declare new variable
                                aVar = (variable*)malloc(sizeof(variable));
                                aVar->dType = atype;
                                aVar->name = tokens[c+1]->text;
                                currentScope->variables.push_back(aVar);
                                name+="_";
                                name+=atype->name;
                            }
                            c+=3;
                        }
                        else
                        {
                            break;
                        }
                    }

                    if(options::ffunctioninfo)
                    {
                        std::cout << "function name: " << name << std::endl;
                    }

                    popScope();
                }
            }
        }
    }
}