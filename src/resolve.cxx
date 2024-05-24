
/*
 * Created Date: Sunday July 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Wednesday May 22nd 2024 11:30:22 am
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

/** 
 * @file
*/

#include <common.h>
#include <compiler.h>
#include <codegen.h>
#include <error.h>
#include <bits.h>
#include <cmath>
#include <dump.hxx>
#include <issues.hxx>

#define IM_NOT_STUCK 0

using namespace issues;

void printToken(token& t);

std::string getFunctionExpression(std::string name,std::vector<variable*>& args);

std::string* LSS = nullptr;

litop* getLitop(std::string name)
{
    for(litop* l : litops)
        if(l->name == name)
            return l;
    noSuchLitop("",originCoreHere,source(),name);
	return IM_NOT_STUCK;
}

/**
 * @brief 
 * 
 * @callgraph
 * @callergraph
 * 
 * @param dig 
 * @return uint8_t 
 */
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
			compilerBug("expected digit.",originCoreHere,source(),"");
    }
    compilerBug("this code is supposed to be unreachable.",originCoreHere,source(),"");
	return IM_NOT_STUCK;
}

#include <numberSystem.h>
std::string numberSystemNames[256];
uint64_t(*numberSystems[256])(std::string& text, uint64_t* numlen) = {nullptr};
char defaultNumberSystem = 'd';
extern std::string __reqFileVSTC;
extern bool vstcDisableSend;
extern bool isConstExprAssignment;
line compLine(std::string text);
void updateCurrentScope(scope* sc);
function* getTypeCastFunction(type* in, type* out)//? only checks for explicit cast
{
    for(castFunction* i : castFunctions)
        if(i->input == in && i->output == out)
        {
			//std::cout << "\"" << i->input->name << "\" == \"" << in->name << "\" && \"" << i->output->name << "\" == \"" << out->name << "\"" << std::endl;
			//printStacktrace(4);
			return i->func;
		}
		else
		{
			//std::cout << "\"" << i->input->name << "\" != \"" << in->name << "\" || \"" << i->output->name << "\" != \"" << out->name << "\"" << std::endl;
		}
    //std::cout << "no type cast function found!" << std::endl;
	//{
	//	dump("cast in",in,"");
	//	dump("cast out",out,"");
	//}
	if(out->valueType == defaultCharType && in->members.size() > 0)
	{
		//std::cout << "generate stringify function: " << in->name << std::endl;
		//printStacktrace(50);
		//,
		//, generate stringify function
		//,
		{
			std::vector<line> stringify;
			stringify.push_back(compLine(
				"typecast "+out->name+" stringify("+in->name+" obj) {")
			);
			stringify.push_back(compLine(
				"(r14) char* data = \"{\\n\";"
			));
			stringify.push_back(compLine(
				"(r13) char* md;"
			));
			stringify.push_back(compLine(
				"(r12) char* nl = \"\\n\";"
			));
			stringify.push_back(compLine(
				"(rbx) char* fn;"
			));
			for(variable& m : in->members)
			{
				stringify.push_back(compLine(
					"md = char*(obj."+m.name+");"
				));
				stringify.push_back(compLine(
					"fn = \"    "+m.name+": \";"
				));
				stringify.push_back(compLine(
					"data = data+fn+md+nl;"
				));
			}
			stringify.push_back(compLine(
				"data = data + \"}\";"
			));
			stringify.push_back(compLine("return data;"));
			stringify.push_back(compLine("}"));
			#if false
				std::cout << "stringify code: " << std::endl;
				for(line& l : stringify)
				{
					std::cout << "    " << l.text << std::endl;
				}
				std::cout << "<code end>" << std::endl;
			#endif
			scope* cs = currentScope;
			updateCurrentScope(globalScope);
			parse(stringify);
			updateCurrentScope(cs);
		}
		return getTypeCastFunction(in,out);
	}
	else if(out->valueType == defaultCharType)
	{
		std::cerr << "cant automatically stringify type \"" << in->name << "\"" << std::endl;
	}
	function* func = new function;
	func->name = out->name;
	func->parameters = {in};
    noSuchFunction("",originCoreHere,source(),func,{});
	return IM_NOT_STUCK;
}
/**
 * @brief cast a variable to a different type
 * 
 * @callgraph
 * @callergraph
 * 
 * @param in 
 * @param targetType 
 * @return variable* 
 */
variable* typecastVariable(variable* in, type* targetType)
{
    function* castfunc = getTypeCastFunction(in->dataType,targetType);
    std::vector<variable*> args = {in};
    return call(castfunc,args);
}
/**
 * @brief resolves immediate values such as numbers and string literals
 * 
 * @callgraph
 * @callergraph
 * 
 * @param t
 * @return a variable using the appropriate data type for the immediate value or nullptr in case the value cannot be resolved
 * 
 * @warning may return nullptr
 * 
 * @defgroup core
 */
variable* resolveIMM(token& t)
{
    if(options::ddebug || true)
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
                //std::cout << "resolving grave string" << std::endl;
                //grave string
                //LSS = &DataCode.back();
                uint64_t tpos = 1;
                //std::cout << "resolving string: " << t.text << std::endl;
                bool strHasSymbol = false;
                type* charPointerType = getType(defaultCharType->name+"*");
                std::string strSym = getNewName();
                variable* tvar = new variable;
                tvar->storage = storageType::SYMBOL_ADDR;
                tvar->dataType = charPointerType;
                tvar->name = strSym;
                tvar->symbol = strSym;
                bool needsStringConcat = false;
                while(1)
                {
                    if(!strHasSymbol)
                    {
                        if(strSym == "")
                            strSym = getNewName();
                        DataCode.push_back(strSym+":");
                        strHasSymbol = true;
                    }
                    switch(t.text[tpos])
                    {
                        case('`'):
                            DataCode.push_back("\t.byte 0");
                            if(options::asmVerbose >= 2){
                                DataCode.back()+=" # terminate string";
                            }
                            if(needsStringConcat)
                            {
                                variable* estrv = new variable;
                                estrv->storage = storageType::SYMBOL_ADDR;
                                estrv->dataType = charPointerType;
                                estrv->name = strSym;
                                estrv->symbol = strSym;
                                //concat previous string with expression
                                std::vector<variable*> args = {tvar,estrv};
                                std::string funcName = "operator+";
                                function* concatFunction = getFunction(charPointerType,funcName,args);
                                tvar = call(concatFunction,args);
                            }
                            goto endLoop2;
                        case('$'):
                        {
                            switch(t.text[tpos+1])
                            {
                                case('{'):
                                {
                                    needsStringConcat = true;
                                    //${expression}
                                    //end current string
                                    DataCode.push_back("\t.byte 0");
                                    if(options::asmVerbose >= 2){
                                        DataCode.back()+=" # terminate string";
                                    }
                                    strHasSymbol = false;
                                    strSym = "";
                                    //get expression
                                    tpos+=2;
									uint64_t scol = t.tcol+tpos;
                                    uint64_t cbracec = 0;
                                    std::string expression = "";
                                    while(true)
                                    {
                                        switch(t.text[tpos])
                                        {
                                            case(0x00):
                                                unexpectedBufferTermination("expression buffer",originCoreHere,source(currentFile,*t.Line,t));
                                            case('}'):
                                                if(cbracec==0)
                                                    goto expressionEnded;
                                                cbracec--;
                                            case('{'):
                                                cbracec++;
                                            default:
                                                expression.push_back(t.text[tpos]);
                                        }
                                        tpos++;
                                    }
                                    expressionEnded:;
                                    tpos++;
                                    //std::cout << "expression: \"" << expression <<"\""<< std::endl;
									//std::cerr << "expr line: " << t.Line->lineNum << "," << t.Line->text << std::endl;
                                    line L = *t.Line;
                                    L.text = expression;
                                    L.tpos = 0;
									L.lineNum = t.Line->lineNum;
									L.twhitespace = scol;
                                    token exprt = L.nextToken();
                                    variable* rexpr = resolve(exprt);
                                    //cast expression to string (char*)
                                    if(rexpr->dataType != charPointerType)
                                    {
                                        //std::cout << "casting expression to char*" << std::endl;
                                        rexpr = typecastVariable(rexpr,charPointerType);
                                    }
                                    //concat previous string with expression
                                    std::vector<variable*> args = {tvar,rexpr};
                                    std::string funcName = "operator+";
                                    function* concatFunction = getFunction(charPointerType,funcName,args);
                                    tvar = call(concatFunction,args);
                                    break;
                                }
                                default:
                                    goto gsr_default;
                            }
                            break;
                        }
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
								case('$'):
									DataCode.push_back("\t.byte "+std::to_string((uint8_t)'$'));
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
                            gsr_default:;
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
				std::cout << "cock: " << t.text[0] << std::endl;
				noSuchIdentifier("",originCoreHere,source(currentFile,*t.Line,t),t.text);
            break;

        //
        // finish up numeric values
        //
        breakResNumeric:;
        {
            type* vtype = defaultUnsignedIntegerType;
            //send vstc information
            if(options::vstc && currentFile == __reqFileVSTC && !vstcDisableSend && t.lineNum != 0)
            {
                std::cout << "0005\x0c" << t.lineNum <<'\x0c'<< t.tcol <<'\x0c'<< (numhs*2)+numlen <<'\x0c'<<value<<'\x0c'<<numsysname<<'\n';
            }
            if(ttlen == numlen)
                goto skipLitopCheck;
            {
                if(options::ddebug)
                    std::cout << "pre litop value: " <<std::dec<< value << std::endl;
                char* litop_ = (char*)(t.text.c_str()+(numhs*2)+numlen);
                if(options::ddebug)
                    std::cout << "checking for litop: " << litop_ << std::endl;
                litop* l = getLitop(litop_);
				if(options::vstc && currentFile == __reqFileVSTC && !vstcDisableSend && t.lineNum != 0)
				{
					std::cout << "0006\x0c" << t.lineNum <<'\x0c'<< t.tcol+(numhs*2)+numlen <<'\x0c'<< l->name.length() <<'\n';
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
    noSuchIdentifier("",originCoreHere,source(),t.text);
	return IM_NOT_STUCK;
}

#include <colors.h>

void sendVstcToken(token& t);
void makeNewToken(std::string& working, uint64_t i, std::vector<token>& tokens,token& t)
{
	std::cout << "creating new token: \"" << working << "\"" << std::endl;
    if(working == "")
        return;
    token nt;
    nt.text = working;
    nt.Line = t.Line;
    nt.col = t.col+i-working.length();
	nt.tcol = t.tcol+i-working.length();
    nt.lineNum = t.lineNum;
	//std::cerr << "linenum: " << nt.lineNum << std::endl;
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

static void resolve_I(variable*& left, token& __leftHand, token& t)
{
	if(left == nullptr)
    {
		try {
			left = getVariable(__leftHand.text);
		}
		catch(noSuchVariable e){}
	}
    if(left == nullptr)
    {
		try {
        	if(__leftHand.text.size() >= 3)
			{
        		if(__leftHand.text.substr(__leftHand.text.size()-3,__leftHand.text.size()) == "++")
        		{
        		    //TODO: shedule operator++ to be called
        		    left = getVariable(__leftHand.text.substr(0,__leftHand.text.size()-3));
        		}
			}
		}
		catch(noSuchVariable e){}
    }
    if(left == nullptr)
    {
        if(__leftHand.text.back() == ')')
        {
            std::string fname = __leftHand.text.substr(0,__leftHand.text.find_first_of('(')-1);
			uint64_t fstart = __leftHand.text.find_first_of('(')+1;
            __leftHand.text = __leftHand.text.substr(fstart,(__leftHand.text.size()-fstart)-1);
            //left = resolve(__leftHand);
        }
    }
    if(left == nullptr)
    {
		try {
        	char* lt = (char*)__leftHand.text.c_str();
        	while(lt[0] != '[' && lt[0] != 0x00)
        	    lt++;
        	uint64_t len = (uint64_t)lt-(uint64_t)__leftHand.text.c_str();
        	char* arraytext = (char*)calloc(0,len+1);
        	memcpy(arraytext,__leftHand.text.c_str(),len);
        	if(options::ddebug)
        	    std::cout << "array: " << arraytext << std::endl;
        	variable* array = getVariable(arraytext);
        	if(lt[0] != 0x00 && lt[1] != 0x00)
        	{
        	    lt++;
        	    char* it = lt;
        	    while(it[0] != ']' && it[0] != 0x00)
        	        it++;
        	    len = (uint64_t)it-(uint64_t)lt;
        	    char* index = (char*)calloc(0,len+1);
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
        	    left = call(indexOP,args__);
        	}
		}
		catch(noSuchVariable e){}
		catch(noSuchFunction e){}
    }
    if(left == nullptr)
		noSuchIdentifier("",originCoreHere,source(),__leftHand.text);
}
static void resolve_II(variable*& left, token& __leftHand, token& t)
{
	std::cout << "resolve_II: " << __leftHand.text << std::endl;
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
            //std::cout << "dong over: \""  << __leftHand.text <<"\""<< std::endl;
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
				variable* arg = resolve(et);
                args.push_back(arg);
            }
            //std::cout << "fname: " << fname << std::endl;
			function* func = nullptr;
			type* ctype = nullptr;
			try {
				func = getFunction(fname,args);
			}catch(noSuchFunction e){func = nullptr;}
			try {
				ctype = getType(fname);
				if(args.size() == 1)
					func = getTypeCastFunction(args[0]->dataType,ctype);
			}
			catch(noSuchFunction e){func = nullptr;}
			catch(noSuchType e){ctype == nullptr;}
			if(func == nullptr)
			{
				func = new function;
				func->name = fname;
				func->vparams = args;
				noSuchFunction("",originCoreHere,source(),func,{});
			}
            if(options::ddebug)
                std::cout << "calling function " << getFunctionExpression(fname,args) << std::endl;
            left = call(func,args);
        }
    }
}
/**
 * @brief resolves any expression
 * 
 * @callgraph
 * @callergraph
 * 
 * @param t
 * @return a variable using the data type to which the expression resolves or nullptr in case the value cannot be resolved
 *
 * @warning may return nullptr
 * 
 * @defgroup core
 */
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
    {
		//std::cout << "expression: " << t.text << COLOR_FUNCTION << " ("<<t.Line->lineNum<<","<<t.Line->text<<","<<t.lineNum<<")" << COLOR_RESET << std::endl;
		dump("expression",&t.text,"");
		//printStacktrace(50);
	}

    for(uint64_t i=0;i<t.text.length();i++)
    {
        //std::cout << "char: " << t.text[i] << std::endl;
        switch(t.text[i])
        {
            case(0x00):
            case(')'):
                goto endTokenCollector;
            case('`'):
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
				try {
					getType(working);
					working.push_back(t.text[i]);
					break;
				}catch(noSuchType e){}
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
				type* ctype = nullptr;
				function* func = nullptr;
				try {
					func = getFunction(working);
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
					break;
				}catch(noSuchFunction e){}
				try {
					ctype = getType(working);
					std::string expr;
                    //std::cout << "collecting constructor call" << std::endl;
                    while(t.text[i] != ')' && t.text[i]!= 0x00 && i<t.text.length())
                    {
                        expr.push_back(t.text[i]);
                        i++;
                    }
                    if(t.text[i] == ')')
                        expr.push_back(')');
                    working+=expr;
					break;
				}catch(noSuchType e){}

                i++;
                std::string expr;
				uint64_t st = i;
                while(t.text[i] != ')' && t.text[i]!= 0x00 && i<t.text.length())
                {
                    expr.push_back(t.text[i]);
                    i++;
                }
                token et;
				//std::cerr << "t: " << t.text << std::endl;
                et.text = expr;
                et.col = t.col;
                et.Line = t.Line;
                et.type = t.type;
				et.lineNum = t.lineNum;
				et.tcol = st+t.tcol;
                variable* rv = resolve(et);
                if(rv->storage == storageType::IMMEDIATE)
                    working+=std::to_string(rv->immediateValue);
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
			token& __rightHand = tokens[i+1];
			token& __center = tokens[i];
			#if false
				std::cout << "tokens:\n";
				if(left == nullptr)
					dump("left",&__leftHand.text,"    ");
				else
					dump("left",left,"    ");
				if(right == nullptr)
					dump("right",&__rightHand.text,"    ");
				else
					dump("right",right,"    ");
			#endif
			#if true
				std::cout << "tokens: ";
				std::cout << "\"" << ((left == nullptr || true) ? (__leftHand.text) : (left->name)) << "\"";
				std::cout << " | ";
				std::cout << "\"" << ((right == nullptr || true) ? (__rightHand.text) : (right->name)) << "\"";
				std::cout << std::endl;
			#endif
            if(left == nullptr)
            {
                try {
					left = resolve(__leftHand);
				}
				catch(noSuchIdentifier e){left = nullptr;}
            }
            //PRINT_DEBUG
			try {
				right = resolve(__rightHand);
			}
			catch(noSuchIdentifier e)
			{
				right = nullptr;
				//std::cout << "unresolved: " << e.name << std::endl;
				//e.printStackTrace();
			}
            if(left == nullptr || right == nullptr)
            {
                //cant calculate at compile time
                std::vector<variable*> args;
				resolve_I(left,__leftHand, t);
				resolve_I(right,__rightHand, t);
                if(options::ddebug) {
                    std::cout << "left: " << std::hex << (void*)left << std::endl;
                    std::cout << "right: " << std::hex << (void*)right << std::endl;
                    std::cout << "left text: " << __leftHand.text << std::endl;
                    std::cout << "right text: " << __rightHand.text << std::endl;
                }
                args.push_back(left);
                if(tokens[i].text != "++")
                    args.push_back(right);
                std::string fname = "operator"+tokens[i].text;
				if(options::ddebug || true)
                    std::cout << "calling function " << getFunctionExpression(fname,args) << std::endl;
                function* func = getFunction(fname, args);
				if (func->isDeprecated)
					warn(getWarning("deprecated"), t.Line,
						 "call to deprecated function \"" +
							 func->name + "\"");
				if (options::asmVerbose >= 3)
					currentScope->func->code.push_back(
						getIndent() + "# " + t.Line->text);
				left = call(func, args);
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
                //else
                //    compilerBug("invalid operator \""+tokens[i].text+"\"",originCoreHere,source(),"");
                //  -1   0  +1 +2
                // 1024 * 1024 * 1024 * 1024 * 1024
                // i+=2;
            }
        }
		else
			unexpectedTokenType("",originCoreHere,source(currentFile,*(tokens[i].Line),tokens[i]),{3,6,7,11,1});
    }
    TokenSize0:;
    if(tokens.size() == 1)
    {
        token& __leftHand = tokens[0];
		std::cout << "last token: " << __leftHand.text << std::endl;
		try {
			left = getVariable(__leftHand.text);
		}
		catch(noSuchVariable e){}
		try {
			left = resolveIMM(__leftHand);
		}
		catch(noSuchIdentifier e)
		{
			std::cout << "rimm error: "<<std::endl;
			e.printStackTrace();
		}
		if(left == nullptr)
			resolve_II(left,__leftHand, t);
    }
	if(left == nullptr)
	{
		noSuchIdentifier("",originCoreHere,source(currentFile,*t.Line,t),t.text);
	}
    return left;
}
