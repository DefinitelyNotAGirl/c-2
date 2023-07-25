/*
 * Created Date: Monday July 10th 2023
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
#include <compiler.hxx>
#include <util.h>
#include <options.h>

expression* resolveIMM(codechar* token)
{
    uint64_t value = 0;
    uint64_t len = strlen(token);
    bool checkLitop = false;
    uint64_t i = 2;
    switch(token[0])
    {
        case('0'):
            switch(token[1]){
            case('x'):
            {
                //base 16
                resBase16:;
                if(options::dprintTokens)
                    std::cout << "number format: hexadecimal" << std::endl;
                for(i=i;i<len;i++)
                {
                    switch(token[i])
                    {
                        case('0'):
                        case('1'):
                        case('2'):
                        case('3'):
                        case('4'):
                        case('5'):
                        case('6'):
                        case('7'):
                        case('8'):
                        case('9'):
                            value<<=4;
                            value += (token[i] - '0');
                            break;
                        case('a'):
                        case('b'):
                        case('c'):
                        case('d'):
                        case('e'):
                        case('f'):
                            value<<=4;
                            value += (token[i] - 'a'+0xA);
                            break;
                        case('A'):
                        case('B'):
                        case('C'):
                        case('D'):
                        case('E'):
                        case('F'):
                            value<<=4;
                            value += (token[i] - 'A'+0xA);
                            break;

                        default:
                            checkLitop = true;
                            goto endSwitchB16;
                    }
                }
                endSwitchB16:;
                goto breakResNumeric;
            }
            case('b'):
                resBase2:;
                //base 2
                if(options::dprintTokens)
                    std::cout << "number format: binary" << std::endl;
                for(i=i;i<len;i++)
                {
                    switch(token[i])
                    {
                        case('0'):
                            value<<=1;
                            break;
                        case('1'):
                            value<<=1;
                            SETBIT_00(value);
                            break;

                        default:
                            checkLitop = true;
                            goto endSwitchB2;
                    }
                }
                endSwitchB2:;
                goto breakResNumeric;
            case('o'):
                resBase8:;
                //base 8
                if(options::dprintTokens)
                    std::cout << "number format: octal" << std::endl;
                for(i=i;i<len;i++)
                {
                    switch(token[i])
                    {
                        case('0'):
                        case('1'):
                        case('2'):
                        case('3'):
                        case('4'):
                        case('5'):
                        case('6'):
                        case('7'):
                            value *= 8;
                            value += (token[i] - '0');
                            break;
                        default:
                            checkLitop = true;
                            goto endSwitchB8;
                    }
                }
                endSwitchB8:;
                goto breakResNumeric;
            case('d'):
                resBase10:;
                //base 10
                if(options::dprintTokens)
                    std::cout << "number format: decimal" << std::endl;
                for(i=i;i<len;i++)
                {
                    switch(token[i])
                    {
                        case('0'):
                        case('1'):
                        case('2'):
                        case('3'):
                        case('4'):
                        case('5'):
                        case('6'):
                        case('7'):
                        case('8'):
                        case('9'):
                            value*=10;
                            value += (token[i] - '0');
                            break;

                        default:
                            checkLitop = true;
                            goto endSwitchB10;
                    }
                }
                endSwitchB10:;
                goto breakResNumeric;
            case('q'):
                resBase4:;
                //base 4
                if(options::dprintTokens)
                    std::cout << "number format: quaternary" << std::endl;
                for(i=i;i<len;i++)
                {
                    switch(token[i])
                    {
                        case('0'):
                        case('1'):
                        case('2'):
                        case('3'):
                            value *= 4;
                            value += (token[i] - '0');
                            break;

                        default:
                            checkLitop = true;
                            goto endSwitchB4;
                    }
                }
                endSwitchB4:;
                goto breakResNumeric;
            default:
                defFormatNumberRes:;
                i = 0;
                if(options::dprintTokens)
                    std::cout << "number format: default" << std::endl;
                switch(defaultNumberBase)
                {
                    case(2):
                        goto resBase2;
                    case(8):
                        goto resBase8;
                    case(10):
                        goto resBase10;
                    case(4):
                        goto resBase4;
                    case(16):
                        goto resBase16;
                }
                break;
            }
        case('"'):
            //string
            break;
        default:
            if(isDigit(token[0]))
                goto defFormatNumberRes;

        //
        // finish up numeric values
        //
        breakResNumeric:;
        {
            if(!checkLitop)
                goto skipLitopCheck;
            {
                std::cout << "pre litop value: " <<std::dec<< value << std::endl;
                codechar* litop = token+i;
                std::cout << "checking for litop: " << litop << std::endl;
                for(uint64_t I = 0;I<litops.size();I++)
                {
                    if(cstrcmp(litops[I]->name,litop))
                    {
                        if(litops[I]->type == litopType::IMM)
                        {
                            litopIMM* nl = (litopIMM*)litops[I];
                            switch(nl->op)
                            {
                                case(arithType::ADD):
                                    if(nl->rIsSigned)
                                        value+=(int64_t)nl->rightHand;
                                    else
                                        value+=(uint64_t)nl->rightHand;
                                    break;
                                case(arithType::SUB):
                                    if(nl->rIsSigned)
                                        value-=(int64_t)nl->rightHand;
                                    else
                                        value-=(uint64_t)nl->rightHand;
                                    break;
                                case(arithType::MUL):
                                    if(nl->rIsSigned)
                                        value*=(int64_t)nl->rightHand;
                                    else
                                        value*=(uint64_t)nl->rightHand;
                                    break;
                                case(arithType::DIV):
                                    if(nl->rIsSigned)
                                        value/=(int64_t)nl->rightHand;
                                    else
                                        value/=(uint64_t)nl->rightHand;
                                    break;
                            }
                        }
                    }
                }
            }
            skipLitopCheck:;
            std::cout << "value: " <<std::dec<< value << std::endl;
            exprIMM* expr = (exprIMM*)malloc(sizeof(exprIMM));
            expr->type = exprType::IMM;
            expr->vType = (uint64_t)langTypes::u64;
            expr->value = value;
            return (expression*)expr;
        }
    }
    return nullptr;
}

expression* resolveExpression(codechar* token)
{
    expression* res = resolveIMM(token);
    if(res != nullptr)
        return res;
}
