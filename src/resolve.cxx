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

std::string getFunctionExpression(std::string name,std::vector<variable*>& args);

std::string* LSS = nullptr;

litop* getLitop(std::string name)
{
    for(litop* l : litops)
        if(l->name == name)
            return l;
    return nullptr;
}

uint8_t HEXDIGTONUM(char dig)
{
    switch(dig)
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
            return (dig - '0');
        case('a'):
        case('b'):
        case('c'):
        case('d'):
        case('e'):
        case('f'):
            return (dig - 'a'+0xA);
        case('A'):
        case('B'):
        case('C'):
        case('D'):
        case('E'):
        case('F'):
            return (dig - 'A'+0xA);
        default:
            error::genericError(0x3001);
    }
    return 0;
}

#include <numberSystem.h>
std::string numberSystemNames[256];
uint64_t(*numberSystems[256])(std::string& text, uint64_t* numlen) = {nullptr};
char defaultNumberSystem = 'd';
extern std::string __reqFileVSTC;
extern bool vstcDisableSend;
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
    uint64_t numlen = 0;
    uint64_t ttlen = t.text.length();
    bool numhs = false;
    std::string numsysname;
    switch(t.text[0])
    {
        case('0'):
            if(numberSystems[(uint64_t)t.text[1]] != nullptr)
            {
                std::string text = t.text.substr(2);
                value = numberSystems[(uint64_t)t.text[1]](text,&numlen);
                ttlen -= 2;
                numhs = true;
                numsysname = numberSystemNames[(uint64_t)t.text[1]];
            }
            else
            {
                resNumDefault:;
                std::string text = t.text;
                value = numberSystems[(uint64_t)defaultNumberSystem](text,&numlen);
                numsysname = numberSystemNames[(uint64_t)defaultNumberSystem];
            }
            goto breakResNumeric;
        case('`'):{
                //grave string
                std::string strSym = getNewName();
                DataCode.push_back(strSym+":");
                //LSS = &DataCode.back();
                uint64_t tpos = 1;
                //std::cout << "resolving string: " << t.text << std::endl;
                while(1)
                {
                    switch(t.text[tpos])
                    {
                        case('`'):
                            DataCode.push_back("\t.byte 0");
                            if(options::asmVerbose >= 2){
                                DataCode.back()+=" # terminate string";
                            }
                            goto endLoop2;
                        case('\\'):
                            tpos++;
                            switch(t.text[tpos])
                            {
                                case('`'):
                                    DataCode.push_back("\t.byte "+std::to_string((uint8_t)t.text[tpos]));
                                    if(options::asmVerbose >= 2){
                                        DataCode.back()+=" # '";
                                        DataCode.back().push_back(t.text[tpos]);
                                        DataCode.back()+="'";
                                    }
                                    tpos++;
                                    break;
                                case('n'):
                                    DataCode.push_back("\t.byte 10");//line feed
                                    tpos++;
                                    break;
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
                                case('A'):
                                case('B'):
                                case('C'):
                                case('D'):
                                case('E'):
                                case('F'):
                                case('a'):
                                case('b'):
                                case('c'):
                                case('d'):
                                case('e'):
                                case('f'):
                                    switch(t.text[tpos+1])
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
                                        case('A'):
                                        case('B'):
                                        case('C'):
                                        case('D'):
                                        case('E'):
                                        case('F'):
                                        case('a'):
                                        case('b'):
                                        case('c'):
                                        case('d'):
                                        case('e'):
                                        case('f'):
                                        {
                                            uint64_t num = HEXDIGTONUM(t.text[tpos]);
                                            num *= 16;
                                            num += HEXDIGTONUM(t.text[tpos+1]);
                                            DataCode.push_back("\t.byte "+std::to_string(num));//line feed
                                            tpos+=2;
                                            break;
                                        }
                                        default:
                                        {
                                            uint64_t num = HEXDIGTONUM(t.text[tpos]);
                                            DataCode.push_back("\t.byte "+std::to_string(num));//line feed
                                            tpos+=1;
                                            break;
                                        }
                                    }
                                    break;
                            }
                            break;
                        case(0x00):
                            goto endLoop2;
                        default:
                            DataCode.push_back("\t.byte "+std::to_string((uint8_t)t.text[tpos]));
                            if(options::asmVerbose >= 2){
                                DataCode.back()+=" # '";
                                DataCode.back().push_back(t.text[tpos]);
                                DataCode.back()+="'";
                            }
                            tpos++;
                    }
                }
                endLoop2:;
                variable* tvar = new variable;
                tvar->storage = storageType::SYMBOL_ADDR;
                tvar->dataType = getType(defaultCharType->name+"*");
                tvar->name = strSym;
                tvar->symbol = strSym;
                return tvar;
                break;
                }
        case('"'):{
            //string
            std::string strSym = getNewName();
            DataCode.push_back(strSym+":");
            //LSS = &DataCode.back();
            uint64_t tpos = 1;
            //std::cout << "resolving string: " << t.text << std::endl;
            while(1)
            {
                switch(t.text[tpos])
                {
                    case('"'):
                        DataCode.push_back("\t.byte 0");
                        if(options::asmVerbose >= 2){
                            DataCode.back()+=" # terminate string";
                        }
                        goto endLoop1;
                    case('\\'):
                        tpos++;
                        switch(t.text[tpos])
                        {
                            case('"'):
                                DataCode.push_back("\t.byte "+std::to_string((uint8_t)t.text[tpos]));
                                if(options::asmVerbose >= 2){
                                    DataCode.back()+=" # '";
                                    DataCode.back().push_back(t.text[tpos]);
                                    DataCode.back()+="'";
                                }
                                tpos++;
                                break;
                            case('n'):
                                DataCode.push_back("\t.byte 10");//line feed
                                tpos++;
                                break;
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
                            case('A'):
                            case('B'):
                            case('C'):
                            case('D'):
                            case('E'):
                            case('F'):
                            case('a'):
                            case('b'):
                            case('c'):
                            case('d'):
                            case('e'):
                            case('f'):
                                switch(t.text[tpos+1])
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
                                    case('A'):
                                    case('B'):
                                    case('C'):
                                    case('D'):
                                    case('E'):
                                    case('F'):
                                    case('a'):
                                    case('b'):
                                    case('c'):
                                    case('d'):
                                    case('e'):
                                    case('f'):
                                    {
                                        uint64_t num = HEXDIGTONUM(t.text[tpos]);
                                        num *= 16;
                                        num += HEXDIGTONUM(t.text[tpos+1]);
                                        DataCode.push_back("\t.byte "+std::to_string(num));//line feed
                                        tpos+=2;
                                        break;
                                    }
                                    default:
                                    {
                                        uint64_t num = HEXDIGTONUM(t.text[tpos]);
                                        DataCode.push_back("\t.byte "+std::to_string(num));//line feed
                                        tpos+=1;
                                        break;
                                    }
                                }
                                break;
                        }
                        break;
                    case(0x00):
                        goto endLoop1;
                    default:
                        DataCode.push_back("\t.byte "+std::to_string((uint8_t)t.text[tpos]));
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
            tvar->storage = storageType::SYMBOL_ADDR;
            tvar->dataType = getType(defaultCharType->name+"*");
            tvar->name = strSym;
            tvar->symbol = strSym;
            return tvar;
            break;
            }
        default:
            if(isdigit(t.text[0]))
            {
                goto resNumDefault;
            }
            else
                return nullptr;
            break;

        //
        // finish up numeric values
        //
        breakResNumeric:;
        {
            type* vtype = defaultUnsignedIntegerType;
            //send vstc information
            if(options::vstc && currentFile == __reqFileVSTC && !vstcDisableSend)
            {
                std::cout << "0005\x0c" << t.lineNum <<'\x0c'<< t.col <<'\x0c'<< (numhs*2)+numlen <<'\x0c'<<value<<'\x0c'<<numsysname<<'\n';
            }
            //0xABC; ttlen = 5, numlen = 3
            //123; ttlen = 3, numlen = 3
            if(ttlen == numlen)
                goto skipLitopCheck;
            {
                if(options::ddebug)
                    std::cout << "pre litop value: " <<std::dec<< value << std::endl;
                char* litop_ = t.text.c_str()+(numhs*2)+numlen;
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

void sendVstcToken(token& t);
void makeNewToken(std::string& working, uint64_t i, std::vector<token>& tokens,token& t)
{
    if(working == "")
        return;
    token nt;
    nt.text = working;
    nt.Line = t.Line;
    nt.col = t.col+i-working.length();
    nt.lineNum = t.lineNum;
    if((working[0] >= 0x21 && working[0] <= 0x2F) || (working[0] >= 0x3c && working[0] <= 0x3e) || (working[0] == 0x5e))
    {
        //std::cout << "token not sent: \"" << nt.text << "\"" << std::endl;
        nt.type = 3;
    }
    //else
    //{
    //    //sendVstcToken(nt);
    //    //std::cout << "token sent: \"" << nt.text << "\"" << std::endl;
    //}
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
    //if(t.type == 6)
    //{
    //    t.text = "\""+t.text+"\"";
    //}
    std::vector<token> tokens;
    std::string working = "";
    token at = t.Line->nextToken();
    token lt = t;
    //vstcDisableSend = true;
    while(at.type != 0 && at.type != 41 && at.type != 40)
    {
        t.text += at.text;
        //printToken(at);
        lt = at;
        //std::cout << "line: \"" << t.Line->text <<"\""<< std::endl;
        at = t.Line->nextToken();
    }
    //vstcDisableSend = false;

    if(options::ddebug)
        std::cout << "expression: " << t.text << std::endl;

    for(uint64_t i=0;i<t.text.length();i++)
    {
        //std::cout << "char: " << t.text[i] << std::endl;
        switch(t.text[i])
        {
            case(0x00):
            case(')'):
                goto endTokenCollector;
            case('"'):
                working.push_back(t.text[i++]);
                while(1)
                {
                    //std::cout << "char: " << t.text[i] << std::endl;
                    switch(t.text[i])
                    {
                        //case('"'):
                        //    working.push_back(t.text[i]);
                        //    goto endLoop1;
                        //case('\\'):
                        //    i++;
                        //    switch(t.text[i])
                        //    {
                        //        case('"'):
                        //            working.push_back(t.text[i++]);
                        //            break;
                        //    }
                        //    break;
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
                    case('>'):
                        working+="->";
                        i++;
                        break;
                    default:
                        makeNewToken(working,i,tokens,t);
                        working="-";
                        makeNewToken(working,i,tokens,t);
                }
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
    if(!tokens.size())
        goto TokenSize0;
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
                    if(__leftHand.text.size() >= 3){
                    if(__leftHand.text.substr(__leftHand.text.size()-3,__leftHand.text.size()) == "++")
                    {
                        //TODO: shedule operator++ to be called
                        left = getVariable(__leftHand.text.substr(0,__leftHand.text.size()-3));
                    }}
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
                {
                    char* lt = __leftHand.text.c_str();
                    while(lt[0] != '[' && lt[0] != 0x00)
                        lt++;
                    uint64_t len = (uint64_t)lt-(uint64_t)__leftHand.text.c_str();
                    char* arraytext = calloc(0,len+1);
                    memcpy(arraytext,__leftHand.text.c_str(),len);
                    if(options::ddebug)
                        std::cout << "array: " << arraytext << std::endl;
                    variable* array = getVariable(arraytext);
                    if(array == nullptr)
                        goto NOTARRAYRESL_2;
                    if(lt[0] != 0x00 && lt[1] != 0x00)
                    {
                        lt++;
                        char* it = lt;
                        while(it[0] != ']' && it[0] != 0x00)
                            it++;
                        len = (uint64_t)it-(uint64_t)lt;
                        char* index = calloc(0,len+1);
                        memcpy(index,lt,len);
                        if(options::ddebug)
                            std::cout << "index: " << index << std::endl;
                        line indexLine = *t.Line;
                        indexLine.tpos = 0;
                        indexLine.text = index;
                        token indexToken = indexLine.nextToken();
                        variable* index_v = resolve(indexToken);
                        //
                        std::string fname = "operator[]";
                        std::vector<variable*> args__;
                        args__.push_back(array);
                        args__.push_back(index_v);
                        function* indexOP = getFunction(fname,args__);
                        if(indexOP)
                            left = call(indexOP,args__);
                        else
                            error::functionNotFound(*t.Line);
                    }
                }
                NOTARRAYRESL_2:;
                if(left == nullptr)
                    std::cout << "ERROR: cant resolve left expression \"" << __leftHand.text << "\"" << std::endl;
                if(right == nullptr)
                    right = getVariable(__rightHand.text);
                if(right == nullptr)
                {
                    if(__rightHand.text.size() >= 3){
                    if(__rightHand.text.substr(__rightHand.text.size()-3,__rightHand.text.size()) == "++")
                    {
                        //TODO: shedule operator++ to be called
                        right = getVariable(__rightHand.text.substr(0,__rightHand.text.size()-3));
                    }}
                }
                if(right == nullptr)
                {
                    if(__rightHand.text.back() == ')')
                    {
                        std::string fname = __rightHand.text.substr(0,__rightHand.text.find_first_of('('));
                        std::string args = __rightHand.text.substr(__rightHand.text.find_first_of('('),__rightHand.text.size());
                        args = args.substr(1,args.length()-2);
                        std::cout << "fname: "  << fname << std::endl;
                        std::cout << "right over:"  << args << std::endl;
                        line L;
                        L.text = args;
                        std::vector<variable*> _args;
                        token at = L.nextToken();
                        while(at.type != 0)
                        {
                            std::string working;
                            //std::cout << "blub" << std::endl;
                            while(at.type != 42 && at.type != 0)
                            {
                                working+=at.text;
                                //printToken(at);
                                at = L.nextToken();
                            }
                            line al;
                            al.text = working;
                            at = al.nextToken();
                            _args.push_back(resolve(at));
                            at = L.nextToken();
                        }
                        function* func = getFunction(fname,_args);
                        right = call(func,_args);
                    }
                }
                if(right == nullptr)
                {
                    char* lt = __rightHand.text.c_str();
                    while(lt[0] != '[' && lt[0] != 0x00)
                        lt++;
                    uint64_t len = (uint64_t)lt-(uint64_t)__rightHand.text.c_str();
                    char* arraytext = calloc(0,len+1);
                    memcpy(arraytext,__rightHand.text.c_str(),len);
                    if(options::ddebug)
                        std::cout << "array: " << arraytext << std::endl;
                    variable* array = getVariable(arraytext);
                    if(array == nullptr)
                        goto NOTARRAYRESL_3;
                    if(lt[0] != 0x00 && lt[1] != 0x00)
                    {
                        lt++;
                        char* it = lt;
                        while(it[0] != ']' && it[0] != 0x00)
                            it++;
                        len = (uint64_t)it-(uint64_t)lt;
                        char* index = calloc(0,len+1);
                        memcpy(index,lt,len);
                        if(options::ddebug)
                            std::cout << "index: " << index << std::endl;
                        line indexLine = *t.Line;
                        indexLine.tpos = 0;
                        indexLine.text = index;
                        token indexToken = indexLine.nextToken();
                        variable* index_v = resolve(indexToken);
                        //
                        std::string fname = "operator[]";
                        std::vector<variable*> args__;
                        args__.push_back(array);
                        args__.push_back(index_v);
                        function* indexOP = getFunction(fname,args__);
                        if(indexOP)
                            right = call(indexOP,args__);
                        else
                            error::functionNotFound(*t.Line);
                    }
                }
                NOTARRAYRESL_3:;
                if(right == nullptr)
                {
                   //std::cout << "ERROR: cant resolve right expression \"" << __rightHand.text << "\"" << std::endl;
                   error::noSuchIdentifier(__rightHand);
                   return nullptr;
                }

                if(options::ddebug) {
                    std::cout << "left: " << std::hex << (void*)left << std::endl;
                    std::cout << "right: " << std::hex << (void*)right << std::endl;
                    std::cout << "left text: " << __leftHand.text << std::endl;
                    std::cout << "right text: " << __rightHand.text << std::endl;
                }
                args.push_back(left);
                if(tokens[i].text != "++")
                    args.push_back(right);
                if(options::ddebug)
                        std::cout << "calling function " << getFunctionExpression("operator"+tokens[i].text,args) << std::endl;
                std::string fname = "operator"+tokens[i].text;
                function* func = getFunction(fname, args);
				if (func != nullptr) {
					if (func->isDeprecated)
						warn(getWarning("deprecated"), t.Line,
							 "call to deprecated function \"" +
								 func->name + "\"");
					if (options::asmVerbose >= 3)
						currentScope->func->code.push_back(
							getIndent() + "# " + t.Line->text);
                    //if(right->dataType == getType(defaultCharType->name+"*") && func->op == primitiveOP::assign)
                    //{
                    //    *LSS = getIndent()+left->name+":";
                    //}
                    //else
                    //{
                    //                    //    std::cout << right->dataType->name << " != " << defaultCharType->name+"*" << std::endl;
                    //}
					left = call(func, args);
				}
                else
                    error::functionNotFound(*t.Line);
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
                    //std::cout << "invalid operator: " << tokens[i].text << std::endl;
                    //invalid operator
                }
                //  -1   0  +1 +2
                // 1024 * 1024 * 1024 * 1024 * 1024
                // i+=2;
            }
        }
    }
    TokenSize0:;
    if(tokens.size() == 1)
    {
        token& __leftHand = tokens[0];
        //PRINT_DEBUG
        //std::cout << "lefthand: " << __leftHand.text << std::endl;
        left = resolveIMM(__leftHand);
        if(left == nullptr)
        {
            if(__leftHand.text[0] == '\"'){
                left = resolveIMM(__leftHand);
            }
        }
        if(left == nullptr)
        {
            left = getVariable(__leftHand.text);
        }
        if(left == nullptr)
        {
            //std::cout << "array check: " << __leftHand.text << std::endl;
            char* lt = __leftHand.text.c_str();
            while(lt[0] != '[' && lt[0] != 0x00)
                lt++;
            uint64_t len = (uint64_t)lt-(uint64_t)__leftHand.text.c_str();
            if(len == 0)
                goto NOTARRAYRESL_1;
            char* arraytext = calloc(1,len+1);
            memcpy(arraytext,__leftHand.text.c_str(),len);
            if(options::ddebug)
                std::cout << "array: " << arraytext << std::endl;
            variable* array = getVariable(arraytext);
            if(array == nullptr)
                goto NOTARRAYRESL_1;
            if(lt[0] != 0x00 && lt[1] != 0x00)
            {
                lt++;
                char* it = lt;
                while(it[0] != ']' && it[0] != 0x00)
                    it++;
                len = (uint64_t)it-(uint64_t)lt;
                if(len == 0)
                    goto NOTARRAYRESL_1;
                char* index = calloc(1,len+1);
                memcpy(index,lt,len);
                if(options::ddebug)
                    std::cout << "index: " << index << std::endl;
                line indexLine = *t.Line;
                indexLine.tpos = 0;
                indexLine.text = index;
                token indexToken = indexLine.nextToken();
                variable* index_v = resolve(indexToken);
                //
                std::string fname = "operator[]";
                std::vector<variable*> args__;
                args__.push_back(array);
                args__.push_back(index_v);
                function* indexOP = getFunction(fname,args__);
                if(indexOP)
                    left = call(indexOP,args__);
                else
                    error::functionNotFound(*t.Line);
            }
        }
        NOTARRAYRESL_1:;
        if(left == nullptr)
        {
            if(__leftHand.text.back() == ')')
            {
                size_t fpo = __leftHand.text.find_first_of('(');
                std::string fname = "";
                if(fpo != std::string::npos)
                    fname = __leftHand.text.substr(0,fpo);
                __leftHand.text = __leftHand.text.substr(__leftHand.text.find_first_of('(')+1,__leftHand.text.size());
                __leftHand.text.pop_back();
                std::vector<variable*> args;
                //std::cout << "left over: \""  << __leftHand.text <<"\""<< std::endl;
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
                            variable* arg = resolve(et);
                            args.push_back(arg);
                            working = "";
                            break;
                        }
                        default:
                            working.push_back(I);
                            break;
                    }
                }
                if(__leftHand.text != "")
                {
                    token et;
                    et.col = 0;
                    et.Line = __leftHand.Line;
                    et.text = working;
                    args.push_back(resolve(et));
                }
                //std::cout << "fname: " << fname << std::endl;
                function* func = getFunction(fname,args);
                if(options::ddebug)
                    std::cout << "calling function " << getFunctionExpression(fname,args) << std::endl;
                left = call(func,args);
                //std::cout << "left dt: " <<std::hex<<(void*) left->dataType << std::endl;
            }
        }
        //std::cout << "blub blub immediate blub blub" << std::endl;
    }

    //if(!left)
    //{
    //    std::cout << "\033[31mERROR:\033[0m could not evaluate expression \""+t.text+"\"" << std::endl;
    //}
    return left;
}
