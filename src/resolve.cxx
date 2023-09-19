/*
 * Created Date: Sunday July 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Sunday July 30th 2023 10:07:58 am
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

#include <common.h>
#include <compiler.h>
#include <codegen.h>
#include <error.h>
#include <bits.h>
#include <cmath>

void printToken(token& t);

std::string getFunctionExpression(std::string name,std::vector<variable*> args);

litop* getLitop(std::string name)
{
    for(litop* l : litops)
        if(l->name == name)
            return l;
    return nullptr;
}

variable* resolveIMM(token& t)
{
    if(options::ddebug)
        std::cout << "resolveIMM: " << t.text << std::endl;
    uint64_t value = 0;
    uint64_t len = t.text.length();
    bool checkLitop = false;
    uint64_t i = 2;
    if(t.text == "")
    {
        variable* ret = new variable;
        ret->immediateValue = 0;
        ret->storage = storageType::IMMEDIATE;
        ret->dataType = defaultUnsignedIntegerType;
        return ret;
    }
    switch(t.text[0])
    {
        case('0'):
            switch(t.text[1]){
            case('x'):
            {
                //base 16
                resBase16:;
                if(options::dprintTokens)
                    std::cout << "number format: hexadecimal" << std::endl;
                for(i=i;i<len;i++)
                {
                    switch(t.text[i])
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
                            value += (t.text[i] - '0');
                            break;
                        case('a'):
                        case('b'):
                        case('c'):
                        case('d'):
                        case('e'):
                        case('f'):
                            value<<=4;
                            value += (t.text[i] - 'a'+0xA);
                            break;
                        case('A'):
                        case('B'):
                        case('C'):
                        case('D'):
                        case('E'):
                        case('F'):
                            value<<=4;
                            value += (t.text[i] - 'A'+0xA);
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
                    switch(t.text[i])
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
                    switch(t.text[i])
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
                            value += (t.text[i] - '0');
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
                    switch(t.text[i])
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
                            value += (t.text[i] - '0');
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
                    switch(t.text[i])
                    {
                        case('0'):
                        case('1'):
                        case('2'):
                        case('3'):
                            value *= 4;
                            value += (t.text[i] - '0');
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
        case('"'):{
            //string
            std::string strSym = getNewName();
            DataCode.push_back(getIndent()+strSym+"_data:");
            uint64_t tpos = 1;
            while(1)
            {
                switch(t.text[tpos])
                {
                    case('"'):
                        DataCode.push_back(getIndent()+"\t.byte 0");
                        if(options::asmVerbose >= 2){
                            DataCode.back()+=" # terminate string";
                        }
                        goto endLoop1;
                    case('\\'):
                        tpos++;
                        switch(t.text[tpos])
                        {
                            case('"'):
                                DataCode.push_back(getIndent()+"\t.byte "+std::to_string((uint8_t)t.text[tpos]));
                                if(options::asmVerbose >= 2){
                                    DataCode.back()+=" # '";
                                    DataCode.back().push_back(t.text[tpos]);
                                    DataCode.back()+="'";
                                }
                                tpos++;
                                break;
                        }
                        break;
                    case(0x00):
                        goto endLoop1;
                    default:
                        DataCode.push_back(getIndent()+"\t.byte "+std::to_string((uint8_t)t.text[tpos]));
                        if(options::asmVerbose >= 2){
                            DataCode.back()+=" # '";
                            DataCode.back().push_back(t.text[tpos]);
                            DataCode.back()+="'";
                        }
                        tpos++;
                }
            }
            endLoop1:;
            variable* tvar = new variable;
            tvar->storage = storageType::SYMBOL;
            tvar->dataType = getType(defaultCharType->name+"*");
            tvar->name = strSym;
            tvar->symbol = strSym+"_data";
            return tvar;
            break;
            }
        default:
            if(isdigit(t.text[0]))
                goto defFormatNumberRes;
            else
                return nullptr;
            break;

        //
        // finish up numeric values
        //
        breakResNumeric:;
        {
            type* vtype = defaultUnsignedIntegerType;
            if(!checkLitop)
                goto skipLitopCheck;
            {
                if(options::ddebug)
                    std::cout << "pre litop value: " <<std::dec<< value << std::endl;
                char* litop_ = t.text.c_str()+i;
                if(options::ddebug)
                    std::cout << "checking for litop: " << litop_ << std::endl;
                litop* l = getLitop(litop_);
                if(l == nullptr)
                {
                    error::noSuchLitop(t,i);
                    return nullptr;
                }
                switch(l->op)
                {
                    case(shortOP::ADD): 
                        value+=l->value;
                        break;
                    case(shortOP::SUB): 
                        value-=l->value;
                        break;
                    case(shortOP::MUL): 
                        value*=l->value;
                        break;
                    case(shortOP::DIV): 
                        value/=l->value;
                        break;
                    default:
                        //invalid
                        break;
                }
            }
            skipLitopCheck:;
            if(options::ddebug)
                std::cout << "value: " <<std::dec<< value << std::endl;
            variable* tvar = new variable;
            tvar->storage = storageType::IMMEDIATE;
            tvar->dataType = vtype;
            tvar->name = getNewName();
            tvar->immediateValue = value;
            return tvar;
        }
    }
    return nullptr;
}

void makeNewToken(std::string& working, uint64_t i, std::vector<token>& tokens,token& t)
{
    token nt;
    nt.text = working;
    nt.Line = t.Line;
    nt.col = t.col+i-working.length();
    if((working[0] >= 0x21 && working[0] <= 0x2F) || (working[0] >= 0x3c && working[0] <= 0x3e) || (working[0] == 0x5e))
        nt.type = 3;

    tokens.push_back(nt);

    working = "";
}

variable* resolve(token& t)
{
    if(options::ddebug)
    {
        std::cout << "resolving token: " << t.text << std::endl;
        printToken(t);
    }
    if(t.type == 6)
    {
        t.text = "\""+t.text+"\"";
    }
    std::vector<token> tokens;
    std::string working = "";
    token at = t.Line->nextToken();
    token lt = t;
    while(at.type != 0 && at.type != 41)
    {
        t.text += at.text;
        //printToken(at);
        lt = at;
        at = t.Line->nextToken();
    }

    if(options::ddebug)
        std::cout << "expression: " << t.text << std::endl;

    for(uint64_t i=0;i<t.text.length();i++)
    {
        switch(t.text[i])
        {
            case(0x00):
            case(')'):
                goto endTokenCollector;
            case('"'):
                working.push_back(t.text[i++]);
                while(1)
                {
                    switch(t.text[i])
                    {
                        case('"'):
                            working.push_back(t.text[i++]);
                            goto endLoop1;
                        case('\\'):
                            i++;
                            switch(t.text[i])
                            {
                                case('"'):
                                    working.push_back(t.text[i++]);
                                    break;
                            }
                            break;
                        case(0x00):
                            goto endLoop1;
                        default:
                            working.push_back(t.text[i++]);
                    }
                }
                endLoop1:;
                break;
            case('+'):
                switch(t.text[i+1])
                {
                    case('+'):
                        makeNewToken(working,i,tokens,t);
                        working="++";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="+=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="+";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('-'):
                switch(t.text[i+1])
                {
                    case('-'):
                        makeNewToken(working,i,tokens,t);
                        working="--";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="-=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="-";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('*'):
                switch(t.text[i+1])
                {
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="*=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="*";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('/'):
                switch(t.text[i+1])
                {
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="/=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="/";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('='):
                switch(t.text[i+1])
                {
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="==";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="=";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('<'):
                switch(t.text[i+1])
                {
                    case('<'):
                        switch(t.text[i+2])
                        {
                            case('='):
                                makeNewToken(working,i,tokens,t);
                                working="<<=";
                                i+=2;
                                makeNewToken(working,i,tokens,t);
                                break;
                            default:
                                makeNewToken(working,i,tokens,t);
                                working="<<";
                                i++;
                        }
                        makeNewToken(working,i,tokens,t);
                        break;
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="<=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="<";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('>'):
                switch(t.text[i+1])
                {
                    case('>'):
                        switch(t.text[i+2])
                        {
                            case('='):
                                makeNewToken(working,i,tokens,t);
                                working=">>=";
                                i+=2;
                                makeNewToken(working,i,tokens,t);
                                break;
                            default:
                                makeNewToken(working,i,tokens,t);
                                working=">>";
                                i++;
                        }
                        makeNewToken(working,i,tokens,t);
                        break;
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working=">=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working=">";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('!'):
                switch(t.text[i+1])
                {
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="!=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="!";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('&'):
                switch(t.text[i+1])
                {
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="&=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    case('&'):
                        makeNewToken(working,i,tokens,t);
                        working="&&";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="&";
                }
                makeNewToken(working,i,tokens,t);
                break;
            case('|'):
                switch(t.text[i+1])
                {
                    case('='):
                        makeNewToken(working,i,tokens,t);
                        working="|=";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    case('|'):
                        makeNewToken(working,i,tokens,t);
                        working="||";
                        i++;
                        makeNewToken(working,i,tokens,t);
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="|";
                }
                break;
            case('('):{
                //collect expression in parentheses
                //std::cout << "collecting parentheses" << std::endl;
                function* func = getFunction(working);
                if(func != nullptr)
                {
                    std::string expr;
                    //std::cout << "collecting function call" << std::endl;
                    while(t.text[i] != ')' && t.text[i]!= 0x00 && i<t.text.length())
                    {
                        expr.push_back(t.text[i]);
                        i++;
                    }
                    if(t.text[i] == ')')
                        expr.push_back(')');
                    working+=expr;
                }
                else
                {
                    i++;
                    std::string expr;
                    while(t.text[i] != ')' && t.text[i]!= 0x00 && i<t.text.length())
                    {
                        expr.push_back(t.text[i]);
                        i++;
                    }
                    token et;
                    et.text = expr;
                    et.col = t.col;
                    et.Line = t.Line;
                    et.type = t.type;
                    variable* rv = resolve(et);
                    if(rv != nullptr)
                    {
                        if(rv->storage == storageType::IMMEDIATE)
                            working+=std::to_string(rv->immediateValue);
                    }
                }
                break;
            }
            case('\t'):
            case('\n'):
            case(' '):
            {
                makeNewToken(working,i,tokens,t);
                break;
            }
            default:
                working.push_back(t.text[i]);
        }
    }
    endTokenCollector:;
    makeNewToken(working,t.text.length(),tokens,t);

    //operator priorities:
    // 1. ^
    // 2. * / %
    // 3. + - < > | & !

    //for(uint64_t i=0;i<tokens.size();i++)
    //    std::cout << "token: " << tokens[i].text << std::endl;

    
    variable* left = nullptr;
    variable* right = nullptr;
    for(uint64_t i=1;i<tokens.size()-1;i+=2)
    {
        if(
            tokens[i].type == 3
            || tokens[i].type == 6
            || tokens[i].type == 7
            || tokens[i].type == 11
            || tokens[i].type == 1
        )
        {
            token& __leftHand = tokens[i-1];
            if(left == nullptr)
            {
                __leftHand = tokens[i-1];
                //std::cout << "lh: " << __leftHand.text << std::endl;
                left = resolveIMM(__leftHand);
            }
            token& __rightHand = tokens[i+1];
            //PRINT_DEBUG
            right = resolveIMM(__rightHand);
            if(left == nullptr || right == nullptr)
            {
                //cant calculate at compile time
                std::vector<variable*> args;
                if(left == nullptr)
                    left = getVariable(__leftHand.text);
                if(left == nullptr)
                {
                    if(__leftHand.text.substr(__leftHand.text.size()-3,__leftHand.text.size()) == "++")
                    {
                        //TODO: shedule operator++ to be called
                        left = getVariable(__leftHand.text.substr(0,__leftHand.text.size()-3));
                    }
                }
                if(left == nullptr)
                {
                    if(__leftHand.text.back() == ')')
                    {
                        std::string fname = __leftHand.text.substr(0,__leftHand.text.find_first_of('(')-1);
                        __leftHand.text = __leftHand.text.substr(__leftHand.text.find_first_of('('),__leftHand.text.size()-1);
                        std::cout << "left over:"  << __leftHand.text << std::endl;
                    }
                }
                if(left == nullptr)
                    std::cout << "ERROR: cant resolve expression \"" << __leftHand.text << "\"" << std::endl;
                if(right == nullptr)
                {
                    right = getVariable(__rightHand.text);
                    if(right == nullptr)
                    {
                        if(__rightHand.text.substr(__rightHand.text.size()-3,__rightHand.text.size()) == "++")
                        {
                            //TODO: shedule operator++ to be called
                            right = getVariable(__rightHand.text.substr(0,__rightHand.text.size()-3));
                        }
                    }
                    if(right == nullptr)
                        std::cout << "ERROR: cant resolve expression \"" << __rightHand.text << "\"" << std::endl;
                }
                if(options::ddebug) {
                    std::cout << "left: " << std::hex << (void*)left << std::endl;
                    std::cout << "right: " << std::hex << (void*)right << std::endl;
                }
                if(tokens[i].text != "++")
                    args.push_back(right);
                args.push_back(left);
                function* func = getFunction("operator"+tokens[i].text, args);
				if (func != nullptr) {
					if (func->isDeprecated)
						warn(getWarning("deprecated"), t.Line,
							 "call to deprecated function \"" +
								 func->name + "\"");
					if (options::asmVerbose >= 3)
						currentScope->func->code.push_back(
							getIndent() + "# " + t.Line->text);
                    if(options::ddebug)
                        std::cout << "calling function " << getFunctionExpression(func->name,args) << std::endl;
					left = call(func, args);
				}
            }
            else
            {
                if(tokens[i].text == "^")
                {
                    //exp
                    left->immediateValue=pow(left->immediateValue,right->immediateValue);
                }
                else if(tokens[i].text == "*")
                    left->immediateValue *= right->immediateValue;
                else if(tokens[i].text == "/")
                    left->immediateValue /= right->immediateValue;
                else if(tokens[i].text == "+")
                    left->immediateValue += right->immediateValue;
                else if(tokens[i].text == "-")
                    left->immediateValue -= right->immediateValue;
                else if(tokens[i].text == "%")
                    left->immediateValue %= right->immediateValue;
                else if(tokens[i].text == "|")
                    left->immediateValue |= right->immediateValue;
                else if(tokens[i].text == "&")
                    left->immediateValue &= right->immediateValue;
                else
                {
                    std::cout << "invalid operator: " << tokens[i].text << std::endl;
                    //invalid operator
                }
                //  -1   0  +1 +2
                // 1024 * 1024 * 1024 * 1024 * 1024
                // i+=2;
            }
        }
    }
    if(tokens.size() == 1)
    {
        token& __leftHand = tokens[0];
        //PRINT_DEBUG
        //std::cout << "lefthand: " << __leftHand.text << std::endl;
        left = resolveIMM(__leftHand);
        if(left == nullptr)
        {
            if(__leftHand.text[0] == '\"')
                left = resolveIMM(__leftHand);
        }
        if(left == nullptr)
        {
            left = getVariable(__leftHand.text);
        }
        if(left == nullptr)
        {
            if(__leftHand.text.back() == ')')
            {
                std::string fname = __leftHand.text.substr(0,__leftHand.text.find_first_of('('));
                __leftHand.text = __leftHand.text.substr(__leftHand.text.find_first_of('(')+1,__leftHand.text.size());
                __leftHand.text.pop_back();
                std::vector<variable*> args;
                //std::cout << "left over: "  << __leftHand.text << std::endl;
                //std::cout << "fname: " << fname << std::endl;

                std::string working;
                for(char I : __leftHand.text)
                {
                    switch(I)
                    {
                        case(','):{
                            token et;
                            et.col = 0;
                            et.Line = __leftHand.Line;
                            et.text = working;
                            args.push_back(resolve(et));
                            working = "";
                            break;
                        }
                        default:
                            working.push_back(I);
                            break;
                    }
                }
                token et;
                et.col = 0;
                et.Line = __leftHand.Line;
                et.text = working;
                args.push_back(resolve(et));
                function* func = getFunction(fname,args);
                if(options::ddebug)
                    std::cout << "calling function " << getFunctionExpression(fname,args) << std::endl;
                left = call(func,args);
                //std::cout << "left dt: " <<std::hex<<(void*) left->dataType << std::endl;
            }
        }
        //std::cout << "blub blub immediate blub blub" << std::endl;
    }
    else
    {
        //std::cout << "\"cum!\" - viper" << std::endl;
        //std::cout << "tokens size: " <<std::dec<< tokens.size() << std::endl;
        //for(token& i : tokens)
        //{
        //    printToken(i);
        //}
    }

    if(options::ddebug)
    {
        //if(left != nullptr)
        //    std::cout << "result: " << left->immediateValue << std::endl;
        //else
        //    std::cout << "no result, left == nullptr" << std::endl;
    }

    return left;
}