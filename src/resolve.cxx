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

bool isConstExpr(std::string& token)
{
}

expression* resolveIMM(codechar* token)
{
    switch(token[0])
    {
        case('0'):
            case('x'):
            {
                //base 16
                uint64_t value = 0;
                uint64_t len = strlen(token);
                bool checkLitop = false;
                uint64_t i = 2;
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
                            value += (token[i] - '0');
                            value<<=4;
                            break;
                        case('a'):
                        case('b'):
                        case('c'):
                        case('d'):
                        case('e'):
                        case('f'):
                            value += (token[i] - 'a');
                            value<<=4;
                            break;
                        case('A'):
                        case('B'):
                        case('C'):
                        case('D'):
                        case('E'):
                        case('F'):
                            value += (token[i] - 'A');
                            value<<=4;
                            break;

                        default:
                            checkLitop = true;
                            goto endSwitch;
                    }
                }
                endSwitch:;
                value>>=4;
                if(!checkLitop)
                    goto skipLitopCheck;
                {
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
                std::cout << "value: " <<std::hex<< value << std::endl;
                break;
            }
            case('b'):
                //base 2
                break;
            case('o'):
                //base 8
                break;
            case('d'):
                //base 10
                break;
            case('q'):
                //base 4
                break;
        case('"'):
            //string
            break;
    }
    return nullptr;
}

expression* resolveExpression(codechar* token)
{
    expression* res = resolveIMM(token);
    if(res != nullptr)
        return res;
}
