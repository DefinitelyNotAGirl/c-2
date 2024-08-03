/*
 * Created Date: Sunday July 30th 2023
 * Author: Lilith
 * -----
 * Last Modified: Fri Aug 02 2024
 * Modified By: Lilith
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
#include <error.h>
#include <bits.h>
#include <cmath>
#include <dump.hxx>
#include <issues.hxx>
#include <SMU.h>
#include <cgu.h>
#include <output.hxx>
#include <ELF64.hxx>
#include <Parser.hxx>

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

extern variable* call(function* func,std::vector<variable*> args);

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
function* getTypeCastFunction(type* in, type* out)//? only checks for explicit cast
{
    for(castFunction* i : castFunctions){
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
			Entity::updateCurrentScope(globalScope);
			parse::Lines(stringify);
			Entity::updateCurrentScope(cs);
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

#include <iomanip>
/**
 * @brief dumps a string and it's memory content in a table-style format
 * 
 * @param str string to dump
 */
static void dumpString(const std::string& str) {
    // Print all characters on the first line
    for (char c : str) {
        std::cout << ' ' << c << " ";
    }
    std::cout << std::endl;

    // Print the memory contents in hexadecimal format on the second line
    for (unsigned char c : str) {
        std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c) << ' ';
    }
    std::cout << std::endl;  // Move to the next line

    // Reset stream to decimal mode
    std::cout << std::dec;
    std::cout << std::endl;  // Move to the next line
}

/**
 * @brief resolves integer immediates
 * 
 * @param t 
 * @return variable*
 * @throw noSuchLitop
 */
static variable* resolveInteger(token& t)
{
	if(!(isdigit(t.text[0])))
		return nullptr;
	uint64_t(*numberSystem)(std::string &text, uint64_t *numlen);
	char ns = defaultNumberSystem;
	bool numhs = 0;
	if(t.text.length() >= 2) {
		if((t.text[0] == '0') && (!isdigit(t.text[1]))) {
			numberSystem = numberSystems[t.text[1]];
			ns = t.text[1];
			numhs = 1;
		} else {
			numberSystem = numberSystems[defaultNumberSystem];
			ns = defaultNumberSystem;
		}
	} else {
		numberSystem = numberSystems[defaultNumberSystem];
		ns = defaultNumberSystem;
	}
	if(numberSystem == nullptr)
		noSuchNumberSystem("",originCoreHere,source(currentFile,*t.Line,t),std::string({ns}));
	std::string numberText = t.text.substr(numhs*2);
	uint64_t numberLength = 0;
	uint64_t value = numberSystem(numberText,&numberLength);
	if((t.text.length()-(numhs*2)) == (numberLength))
		goto skipLitopCheck;
	if((t.text.length()-(numhs*2)) <= (numberLength))
		compilerBug("(t.text.length()-2) <= (numberLength)",originCoreHere,source(currentFile,*t.Line,t),"");
	//,
	//, check for litop
	//,
	{
		std::string litopName = numberText.substr(numberLength);
		litop* op = getLitop(litopName);
		switch(op->op)
		{
			case(shortOP::ADD):
				value+=op->value;
				break;
			case(shortOP::SUB):
				value-=op->value;
				break;
			case(shortOP::MUL):
				value*=op->value;
				break;
			case(shortOP::DIV):
				value/=op->value;
				break;
			default:
				compilerBug("unimplemented",originCoreHere,source(currentFile,*t.Line,t),"");
		}
	}
	skipLitopCheck:;
	variable* var = new variable;
	var->name = getNewVariableName();
	var->dataType = defaultUnsignedIntegerType;
	var->storageArch = Architecture::storage_IntegerImmediate;
	var->storage = (void*)value;
	return var;
}

/**
 * @brief resolves any string expression to a variable
 * 
 * @param t 
 * @return variable* 
 * @throw noSuchIdentifier
 * @throw noSuchLitop
 */
static variable* resolveString(token& t)
{
	if(t.text.size() < 2)
		return nullptr;
	if(!((t.text[0] == '`') || (t.text[0] == '"')))
		return nullptr;
	if(t.text.back() != t.text.front())
		return nullptr;
	uint64_t i = 0;
	std::string text = t.text.substr(1,t.text.length()-2);
	std::string strSym = getNewName();
	type* charPointerType = getType(defaultCharType->name+"*");
	variable* str = new variable;
	str->dataType = charPointerType;
	str->symbol = getNewName();
	str->name = getNewVariableName();
	data.placeSymbol(SymbolType::LocalVariable,0,str->symbol);
	//+
	//+	amd64 storage
	//+
	str->storageArch = currentArchitecture;
	if(currentArchitecture == Architecture::AMD64)
	{
		amd64::VariableStorage* storage = new amd64::VariableStorage;
		str->storage = (void*)storage;
		storage->mode = amd64::StorageMode::DirectImmediate;
		storage->immediate = ImmediateValue(str->symbol);
	}
	else compilerBug("unsupported architecture");
	while(i < (text.length()-1))
	{ 
		char c = text[i];
		switch(c)
		{
			case(0x00):
				unexpectedBufferTermination("",originCoreHere,source());
			case('\\'):
			{
				if(!(text.length()>(i+1)))
					unexpectedBufferTermination("",originCoreHere,source());
				char ec = text[i+1];
				switch(ec)
				{
					case('n'):
						data.push({0x0A});
						i++;
						break;
					case('t'):
						data.push({0x09});
						i++;
						break;
					case('v'):
						data.push({0x0B});
						i++;
						break;
					case('r'):
						data.push({0x0D});
						i++;
						break;
					case('"'):
						data.push({0x22});
						i++;
						break;
					case('`'):
						data.push({0x60});
						i++;
						break;
					default:
						if(isdigit(ec))
						{
							if((text.length()>(i+2)) && isdigit(text[i+2]))
							{
								byte b = (HEXDIGTONUM(text[i+1])<<4)|(HEXDIGTONUM(text[i+2])<<0);
								data.push({b});
								i+=2;
							}
							else
							{
								byte b = (HEXDIGTONUM(text[i+1])<<0);
								data.push({b});
								i++;
							}
						}
						else
							compilerBug("unimplimented escape sequence");
				}
				break;
			}
			case('$'):
			{
				if(!(text.length()>(i+1)))
					goto resstr_default;
				if(text[i+1] != '{')
					goto resstr_default;
				line exprl = *t.Line;
				exprl.text = "";
				exprl.tpos = 0;
				uint64_t bracec = 0;
				i+=2;
				while(true)
				{
					char c = text[i];
					if(c == 0x00)
						unexpectedBufferTermination("",originCoreHere,source());
					if(c == '}' && bracec == 0)
						break;
					if(c == '{')
						bracec++;
					if(c == '}')
						bracec--;
					exprl.text.push_back(c);
					i++;
				}
				token exprt = exprl.nextToken();
				variable* ap = resolve(exprt);
				if(ap->dataType != charPointerType)
					ap = typecastVariable(ap,charPointerType);
				std::vector<variable*> args = {str,ap};
				std::string funcName = "operator+";
				function* concatFunction = getFunction(charPointerType,funcName,args);
				str = call(concatFunction,args);
				break;
			}
			default:
				resstr_default:;
				data.push({(byte)c});
		}
		i++;
	}
	data.push({(byte)0x00});
	data.symbols.back().size = data.size() - data.symbols.back().value;
	return str;
}

/**
 * @brief resolves any expression starting at token& ft
 *
 * @param ft
 * @return variable* 
 * @throws noSuchIdentifier
 * @throws noSuchFunction
 * @throws noSuchVariable
 * @throws noSuchType
 * @throws compilerBug
 * @throws unexpectedBufferTermination
 * @throws unexpectedTokenType
 */
variable* resolve(token& ft)
{
	line* L = ft.Line;
	//dump("resolving token",&L->text,"");
	//,
	//, collect expression tokens
	//,
	std::vector<token> tokens = {ft};
	std::vector<token> outputList;
	std::vector<token> stack;
	uint64_t II = 0;
	std::string expression = ft.text;
	while(true)
	{
		token t = L->nextToken();
		switch(t.type)
		{
			case(00):
			case(41):
				goto rsb0;
			default:
				expression+=t.text;
				tokens.push_back(t);
		}
	}
	rsb0:;
	if(tokens.size() == 1)
		goto skipShuntingYard;
	//vlistdump("expression tokens: ",&tokens,"");
	//,
	//, shunting yard algorithm
	//,
	{
		for(uint64_t i = 0;i<tokens.size();i++)
		{
			token& t = tokens[i];
			if(t.type == 100)
				continue;
			if((i+1) < tokens.size())
			{
				if(t.text == "<"){
					if(tokens[i+1].text == "<"){
						t.text = "<<";
						tokens[i+1].type = 100;
					}
				}
				else if(t.text == ">"){
					if(tokens[i+1].text == ">"){
						t.text = ">>";
						tokens[i+1].type = 100;
					}
				}
				else if(t.text == "="){
					if(tokens[i+1].text == "="){
						t.text = "==";
						tokens[i+1].type = 100;
					}
				}
				else if(t.text == "=="){
					if(tokens[i+1].text == "="){
						t.text = "===";
						tokens[i+1].type = 100;
					}
				}
			}
			//.
			//. PR-0
			//.
			if(
				(t.text == "^")
			)
			{
				while(
					(stack.size() > 0)
					&&
					(
						(stack.back().text == "^")
					)
				)
				{
					outputList.push_back(stack.back());
					stack.pop_back();
				}
				stack.push_back(t);
			}
			//.
			//. PR-1
			//.
			if(
				(t.text == "*")
				||
				(t.text == "/")
				||
				(t.text == "%")
			)
			{
				while(
					(stack.size() > 0)
					&&
					(
						(stack.back().text == "^")
						||
						(stack.back().text == "*")
						||
						(stack.back().text == "/")
						||
						(stack.back().text == "%")
					)
				)
				{
					outputList.push_back(stack.back());
					stack.pop_back();
				}
				stack.push_back(t);
			}
			//.
			//. PR-2
			//.
			if(
				(t.text == "+")
				||
				(t.text == "-")
				||
				(t.text == "±")
				||
				(t.text == "<<")
				||
				(t.text == ">>")
				||
				(t.text == "<")
				||
				(t.text == ">")
				||
				(t.text.back() == '=')
			)
			{
				while(
					(stack.size() > 0)
					&&
					(
						(stack.back().text == "^")
						||
						(stack.back().text == "*")
						||
						(stack.back().text == "/")
						||
						(stack.back().text == "%")
						||
						(stack.back().text == "+")
						||
						(stack.back().text == "-")
						||
						(stack.back().text == "±")
						||
						(stack.back().text == "<<")
						||
						(stack.back().text == ">>")
						||
						(stack.back().text == "<")
						||
						(stack.back().text == ">")
						||
						(stack.back().text.back() == '=')
					)
				)
				{
					outputList.push_back(stack.back());
					stack.pop_back();
				}
				stack.push_back(t);
			}
			//.
			//. left parentheses
			//.
			if(t.text == "(")
			{
				stack.push_back(t);
			}
			//.
			//. right parentheses
			//.
			if(t.text == ")")
			{
				while(stack.back().text != "(")
				{
					outputList.push_back(stack.back());
					stack.pop_back();
				}
				stack.pop_back();
			}
			//.
			//. check for variable,function or number
			//.
			switch(t.type)
			{
				case(6):
					outputList.push_back(t);
					break;
				case(10):
					outputList.push_back(t);
					break;
				case(11):
				{
					token exprt = t;
					uint64_t bracec = 0;
					if(tokens.size() <= i+1)
					{
						dump("i",&i,"");
						vlistdump("tokens",&tokens,"");
						unexpectedBufferTermination("",originCoreHere,source());
					}
					t = tokens[++i];
					exprt.text.append(t.text);
					if(t.type != 30)
						unexpectedTokenType("",originCoreHere,source(currentFile,*t.Line,t),{30});
					while(true)
					{
						t = tokens[++i];
						exprt.text.append(t.text);
						if(t.type == 31 && bracec == 0)
							break;
						if(t.type == 30)
							bracec++;
						if(t.type == 31)
							bracec--;
					}
					outputList.push_back(exprt);
					break;
				}
				case(15):
				{
					token exprt = t;
					uint64_t bracec = 0;
					t = tokens[++i];
					exprt.text.append(t.text);
					if(t.type != 34)
						unexpectedTokenType("",originCoreHere,source(currentFile,*t.Line,t),{34});
					while(true)
					{
						t = tokens[++i];
						exprt.text.append(t.text);
						if(t.type == 35 && bracec == 0)
							break;
						if(t.type == 34)
							bracec++;
						if(t.type == 35)
							bracec--;
					}
					outputList.push_back(exprt);
					break;
				}
				default:
					if(resolveInteger(t) != nullptr)
						outputList.push_back(t);
			}
		}
		while(stack.size() > 0)
		{
			outputList.push_back(stack.back());
			stack.pop_back();
		}
	}
	//,
	//, eval postfix (outputList)
	//,
	{
		while(stack.size() > 0)
			stack.pop_back();
		//std::cout << "#" << std::endl;
		for(token& t : outputList)
		{
			//std::cout << "output list:";for(uint64_t I = II;I<outputList.size();I++){std::cout << " " << outputList[I].text;}std::cout << std::endl;
			//std::cout << "stack:";for(uint64_t I = 0;I<stack.size();I++){std::cout << " " <<stack[I].text;}std::cout << std::endl;
			switch(t.type)
			{
				case(3):
				case(34):
				case(35):
				{
					token op = t;
					if(
						   (t.text == "^")
						|| (t.text == "*")
						|| (t.text == "/")
						|| (t.text == "+")
						|| (t.text == "-")
						|| (t.text == "%")
						|| (t.text == "±")
						|| (t.text == "<<")
						|| (t.text == ">>")
						|| (t.text == "<")
						|| (t.text == ">")
						|| (t.text == "==")
					)
					{
						if(stack.size() < 2)
							compilerBug("stack.size() < 2",originCoreHere,source(),"");
						token op1t = stack.back();stack.pop_back();
						token op2t = stack.back();stack.pop_back();
						line op1l(op1t);
						line op2l(op2t);
						token op1lt = op1l.nextToken();
						token op2lt = op2l.nextToken();
						variable* op1 = resolve(op1lt);
						variable* op2 = resolve(op2lt);
						std::string funcName = "operator"+op.text;
						std::vector<variable*> args = {op2,op1};
						function* func = getFunction(funcName,args);
						compilerBug::error.push([](compilerBug e) -> int {return 1;});
						variable* out;
						try {
							out = call(func,args);
							compilerBug::error.pop();
						} catch(compilerBug e) {
							compilerBug::error.pop();
							e.src = source(currentFile,*t.Line,t);
							issues::invoke(e);
						}
						currentScope->variables.push_back(out);
						stack.push_back(token(out));
					}
					else {
						compilerBug("unimplimented operator: "+t.text,originCoreHere,source(),"");
					}
					break;
				}
				case(10):
				case(11):
				case(15):
				case(6):
				{
					stack.push_back(t);
					break;
				}
				default:
				{
					if(resolveInteger(t) != nullptr)
						stack.push_back(t);
				}
			}
			II++;
		}
		//+
		//+ return
		//+
		{
			if(stack.size() == 0)
				compilerBug("stack.size() == 0, expression: "+expression,originCoreHere,source(),"");
			skipShuntingYard_2:;
			if(stack.back().type == 11)
			{
				std::string funcName;
				uint64_t i = 0;
				while((stack.back().text[i] != 0x00) && (stack.back().text[i] != '(') && (stack.back().text[i] != '<'))
					funcName.push_back(stack.back().text[i++]);
				if(stack.back().text[i] == 0x00)
				{
					dump("funcName",&funcName,"");
					unexpectedBufferTermination("",originCoreHere,source());
				}
				if(stack.back().text[i] == '(')
				{
					//. normal function
					i++;
					std::vector<variable*> args;
					while(true)
					{
						uint64_t cbracec = 0;
						uint64_t sbracec = 0;
						uint64_t rbracec = 0;
						uint64_t abracec = 0;
						std::string arg;
						char c;
						while(true)
						{
							c = stack.back().text[i];
							if(((c == ',') || (c == ')')) && (cbracec == 0) && (sbracec == 0) && (rbracec == 0) && (abracec == 0))
								break;
							if(c == '{')
								cbracec++;
							else if(c == '[')
								sbracec++;
							else if(c == '(')
								rbracec++;
							else if(c == '<')
								abracec++;
							else if(c == '}')
								cbracec--;
							else if(c == ']')
								sbracec--;
							else if(c == ')')
								rbracec--;
							else if(c == '>')
								abracec--;
							arg.push_back(c);
							i++;
						}
						if(arg.size() > 0)
						{
							line argl = *stack.back().Line;
							argl.text = arg;
							argl.tpos = 0;
							token argt = argl.nextToken();
							variable* av = resolve(argt);
							args.push_back(av);
						}
						if(c == ')')
							break;
						i++;
					}
					function* func = getFunction(funcName,args);
					return call(func,args);
				}
				else if(stack.back().text[i] == '<')
				{
					//. builtin function
					i++;
					compilerBug("builtin functions unimplemented",originCoreHere,source(),"");
				}
			}
			noSuchVariable::error.push([](noSuchVariable e) -> int {return 0;});
			variable* var = getVariable(stack.back().text);
			noSuchVariable::error.pop();
			if(var != nullptr)
			{
				return var;
			}
			var = resolveInteger(stack.back());
			if(var == nullptr)
				var = resolveString(stack.back());
			if(var == nullptr)
				noSuchIdentifier("",originCoreHere,source(),stack.back().text);
			return var;
		}
		//.
		//. skipShuntingYard
		//.
		{
			compilerBug("this could should be unreachable",originCoreHere,source(),"");
			skipShuntingYard:;
			stack.push_back(tokens[0]);
			goto skipShuntingYard_2;
		}
	}
}
