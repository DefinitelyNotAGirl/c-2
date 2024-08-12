/*
 * Created Date: Monday July 31st 2023
 * Author: Lilith
 * -----
 * Last Modified: Sat Aug 10 2024
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

#include <compiler.h>
#include <mangling.h>
#include <Parser.hxx>
#include <dump.hxx>
#include <conditions.hxx>
#include <event.hxx>

using smu::InstructionComponent;
using smu::InstructionComponentType;

using namespace issues;

static uint64_t NewNameCount = 0;

std::string getNewName()
{
    std::string mfile;
    for(char i : currentFile)
    {
        switch(i)
        {
            case('.'):
                mfile+=CPE2_SYMBOL_SCOPE_SEP;
                break;
            case('\\'):
                mfile.push_back('_');
                break;
            case('/'):
                mfile.push_back('_');
                break;
            default:
                mfile.push_back(i);
        }
    }
    //return "local"CPE2_SYMBOL_SCOPE_SEP+mfile+CPE2_SYMBOL_SCOPE_SEP+"anonymous"+std::to_string(NewNameCount++);
	return "anonymous"+std::to_string(NewNameCount++);
	//return currentScope->name+CPE2_SYMBOL_SCOPE_SEP+"anonymous"+std::to_string(NewNameCount++);
}

std::string getNewVariableName()
{
    return "__c2_anonymous"+std::to_string(NewNameCount++);
}

uint64_t alignToMultiple(uint64_t value, uint64_t alignment) 
{
    // Ensure alignment is not zero to avoid division by zero
    if (alignment == 0)
        return value;

    // Calculate the remainder when value is divided by alignment
    uint64_t remainder = value % alignment;

    // If remainder is zero, value is already aligned, return value
    if (remainder == 0)
        return value;

    // Calculate the adjustment needed to align value
    uint64_t adjustment = alignment - remainder;

    // Return the aligned value
    return value + adjustment;
}

variable* call(function* func,std::vector<variable*> args)
{
	if(args.size() != func->vparams.size())
		compilerBug("invalid call to variable* call(function* func,std::vector<variable*> args), args.size() != func->vparams.size()");
	if(func != nullptr) {
		Event::Data::Call EventData;
		EventData.func = func;
		EventData.args = &args;
		Event::Call.fire(&EventData);
	}
	//std::cout << "call to function: " << func->expression_ansi() << std::endl;
	if(func->isPrimitive) {
		if(true)
		{
			//attempt to compute at compile time
			switch(func->op)
			{
				case(primitiveOP::add):
				{
					if(args[0]->storageArch == Architecture::storage_IntegerImmediate && args[1]->storageArch == Architecture::storage_IntegerImmediate)
					{
						variable* result = new variable;
						result->storageArch = Architecture::storage_IntegerImmediate;
						result->storage = (void*)((uint64_t)args[0]->storage + (uint64_t)args[1]->storage);
						result->dataType = getType("u64");
						result->name = "____cpe2internalresult";
						return result;
					}
					break;
				}
				case(primitiveOP::mul):
				{
					if(args[0]->storageArch == Architecture::storage_IntegerImmediate && args[1]->storageArch == Architecture::storage_IntegerImmediate)
					{
						variable* result = new variable;
						result->storageArch = Architecture::storage_IntegerImmediate;
						result->storage = (void*)((uint64_t)args[0]->storage * (uint64_t)args[1]->storage);
						result->dataType = getType("u64");
						result->name = "____cpe2internalresult";
						return result;
					}
					break;
				}
				case(primitiveOP::assign):
				{
					if(args[0]->storageArch == Architecture::storage_IntegerImmediate && args[1]->storageArch == Architecture::storage_IntegerImmediate)
					{
						args[0]->storage = args[1]->storage;
						return args[0];
					}
					break;
				}
				case(primitiveOP::SYSCALL):break;
				case(primitiveOP::Less): {
					if(args[0]->storageArch == Architecture::storage_IntegerImmediate && args[1]->storageArch == Architecture::storage_IntegerImmediate)
					{
						bool eval = (uint64_t)args[0]->storage < (uint64_t)args[1]->storage;
						variable* result = new variable;
						result->storageArch = Architecture::storage_IntegerImmediate;
						result->storage = (void*)(eval);
						result->dataType = getType("bool");
						result->name = "____cpe2internalresult";
						ConditionCode.push((Condition)(0xF0 | eval));
						return result;
					}
					break;
				}
				case(primitiveOP::Greater): {
					if(args[0]->storageArch == Architecture::storage_IntegerImmediate && args[1]->storageArch == Architecture::storage_IntegerImmediate)
					{
						bool eval = (uint64_t)args[0]->storage > (uint64_t)args[1]->storage;
						variable* result = new variable;
						result->storageArch = Architecture::storage_IntegerImmediate;
						result->storage = (void*)(eval);
						result->dataType = getType("bool");
						result->name = "____cpe2internalresult";
						ConditionCode.push((Condition)(0xF0 | eval));
						return result;
					}
					break;
				}
				default: {
					if(args[0]->storageArch == Architecture::storage_IntegerImmediate && args[1]->storageArch == Architecture::storage_IntegerImmediate)
						compilerBug("compile time operation not implemented: "+std::string(stringify(func->op)));
				}
					
			}
		}
		//+
		//+ check if any arguments do not have proper storage (storage_IntegerImmediate or such)
		//+
		for(uint64_t i = 0;i<args.size();i++){
			if(args[i]->storageArch == Architecture::storage_IntegerImmediate)
			{
				variable* src = args[i];
				args[i] = new variable(*src);
				args[i]->storageArch = Architecture::AMD64;
				args[i]->storage = (void*)(new amd64::VariableStorage);
				((amd64::VariableStorage*)args[i]->storage)->mode = amd64::StorageMode::DirectImmediate;
				((amd64::VariableStorage*)args[i]->storage)->immediate = ImmediateValue((uint64_t)src->storage);
			}
			else if(args[i]->storageArch == Architecture::storage_member)
			{
				variable* src = args[i];
				args[i] = new variable(*src);
				args[i]->storageArch = Architecture::AMD64;
				args[i]->storage = (void*)(new amd64::VariableStorage);
				if(src->parent->storageArch == Architecture::AMD64)
				{
					//+
					//+	child of value
					//+
					if(src->parent->dataType->valueType == nullptr)
					{
						//+
						//+	[reg+disp0/8/32]
						//+
						if(((amd64::VariableStorage*)src->parent->storage)->mode == amd64::StorageMode::IndirectRegister)
						{
							((amd64::VariableStorage*)args[i]->storage)->mode = amd64::StorageMode::IndirectRegister;
							((amd64::VariableStorage*)args[i]->storage)->reg = ((amd64::VariableStorage*)src->parent->storage)->reg;
							((amd64::VariableStorage*)args[i]->storage)->displacement = ImmediateValue((uint64_t)src->storage);
						}
						else
							compilerBug("unimplemented: child of off-stack object");	
					}
					//+
					//+	child of reference
					//+
					else
					{
						if(((amd64::VariableStorage*)src->parent->storage)->mode == amd64::StorageMode::DirectRegister)
						{
							((amd64::VariableStorage*)args[i]->storage)->mode = amd64::StorageMode::IndirectRegister;
							((amd64::VariableStorage*)args[i]->storage)->reg = ((amd64::VariableStorage*)src->parent->storage)->reg;
							((amd64::VariableStorage*)args[i]->storage)->displacement = ImmediateValue((uint64_t)src->storage);
						}
						else compilerBug("unimplemented: child of non-register reference");
					}
				}
				else
					compilerBug("parent-child architecture combination invalid");
			}
		}
		switch(func->op)
		{
			case(primitiveOP::add):
				return runtime::UnsignedIntegerAddition(args[0],args[1]);
			case(primitiveOP::mul):
				return runtime::UnsignedIntegerMultiplication(args[0],args[1]);
			case(primitiveOP::assign):
				runtime::copy(args[1],args[0]);
				return args[1];
			case(primitiveOP::SYSCALL):
			{
				switch(currentArchitecture)
				{
					case(Architecture::AMD64):
						code->push({
							0x0F,
							amd64::opcode::secondary::syscall
						});
						return nullptr;
						break;
				}
				break;
			}
			case(primitiveOP::Less): {
				if(currentArchitecture == Architecture::AMD64) {
					variable rax = *args[0];
					amd64::VariableStorage raxStore;
					rax.storage = (void*)&raxStore;
					raxStore.mode = amd64::StorageMode::DirectRegister;
					raxStore.reg = amd64::Register::rax;
					variable rdx = *args[1];
					amd64::VariableStorage rdxStore;
					rdx.storage = (void*)&rdxStore;
					rdxStore.mode = amd64::StorageMode::DirectRegister;
					rdxStore.reg = amd64::Register::rdx;
					runtime::copy(args[0],&rax);
					runtime::copy(args[1],&rdx);
					code->push(std::vector<InstructionComponent>({
						amd64::prefix::REX(1,0,0,0),
						amd64::opcode::cmp::r16_32_64__rm16_32_64,
						amd64::modRM(amd64::Register::rax,amd64::Register::rdx)
					}));
					ConditionCode.push(Condition::Less);
					variable* result = new variable;
					result->storageArch = Architecture::storage_condition;
					result->storage = (void*)(Condition::Less);
					result->dataType = getType("bool");
					result->name = "____cpe2internalresult";
					return result;
				}
				break;
			}
			case(primitiveOP::Greater): {
				if(currentArchitecture == Architecture::AMD64) {
					variable rax = *args[0];
					amd64::VariableStorage raxStore;
					rax.storage = (void*)&raxStore;
					raxStore.mode = amd64::StorageMode::DirectRegister;
					raxStore.reg = amd64::Register::rax;
					variable rdx = *args[1];
					amd64::VariableStorage rdxStore;
					rdx.storage = (void*)&rdxStore;
					rdxStore.mode = amd64::StorageMode::DirectRegister;
					rdxStore.reg = amd64::Register::rdx;
					runtime::copy(args[0],&rax);
					runtime::copy(args[1],&rdx);
					code->push(std::vector<InstructionComponent>({
						amd64::prefix::REX(1,0,0,0),
						amd64::opcode::cmp::r16_32_64__rm16_32_64,
						amd64::modRM(amd64::Register::rax,amd64::Register::rdx)
					}));
					ConditionCode.push(Condition::Greater);
					variable* result = new variable;
					result->storageArch = Architecture::storage_condition;
					result->storage = (void*)(Condition::Greater);
					result->dataType = getType("bool");
					result->name = "____cpe2internalresult";
					return result;
				}
				break;
			}
			default:
				compilerBug("runtime operation not implemented: "+std::string(stringify(func->op)));
		}
	} else {
		//+
		//+ check if any arguments do not have proper storage (storage_IntegerImmediate or such)
		//+
		for(uint64_t i = 0;i<args.size();i++){
			if(args[i]->storageArch == Architecture::storage_IntegerImmediate)
			{
				variable* src = args[i];
				args[i] = new variable(*src);
				args[i]->storageArch = Architecture::AMD64;
				args[i]->storage = (void*)(new amd64::VariableStorage);
				((amd64::VariableStorage*)args[i]->storage)->mode = amd64::StorageMode::DirectImmediate;
				((amd64::VariableStorage*)args[i]->storage)->immediate = ImmediateValue((uint64_t)src->storage);
			}
			else if(args[i]->storageArch == Architecture::storage_member)
			{
				variable* src = args[i];
				args[i] = new variable(*src);
				args[i]->storageArch = Architecture::AMD64;
				args[i]->storage = (void*)(new amd64::VariableStorage);
				if(src->parent->storageArch == Architecture::AMD64)
				{
					//+
					//+	child of value
					//+
					if(src->parent->dataType->valueType == nullptr)
					{
						//+
						//+	[reg+disp0/8/32]
						//+
						if(((amd64::VariableStorage*)src->parent->storage)->mode == amd64::StorageMode::IndirectRegister)
						{
							((amd64::VariableStorage*)args[i]->storage)->mode = amd64::StorageMode::IndirectRegister;
							((amd64::VariableStorage*)args[i]->storage)->reg = ((amd64::VariableStorage*)src->parent->storage)->reg;
							((amd64::VariableStorage*)args[i]->storage)->displacement = ImmediateValue((uint64_t)src->storage);
						}
						else
							compilerBug("unimplemented: child of off-stack object");	
					}
					//+
					//+	child of reference
					//+
					else
					{
						compilerBug("unimplemented: child of reference");
					}
				}
				else
					compilerBug("parent-child architecture combination invalid");
			}
		}
		switch(currentArchitecture)
		{
			case(Architecture::AMD64):
			{
				uint64_t pushed = 0;
				std::vector<variable*> saved;
				//+
				//+ check if any arguments need to be moved out of the way
				//+
				for(uint64_t isrc = 0;isrc<args.size();isrc++){
					bool moved = false;
					variable* src = args[isrc];
					if(src->storageArch != currentArchitecture)
						compilerBug("argument architecture mismatch: "+std::to_string((int)src->storageArch)+"("+src->name+") - "+std::to_string((int)currentArchitecture));
					amd64::VariableStorage* storage = (amd64::VariableStorage*)src->storage;
					if(storage->mode != amd64::StorageMode::DirectRegister)
					{
						saved.push_back(nullptr);
						continue;
					}
					for(uint64_t idst = 0;idst<args.size();idst++){
						variable* dst = func->vparams[idst];
						if(dst->storageArch != currentArchitecture)
							compilerBug("argument architecture mismatch");
						if(((amd64::VariableStorage*)dst->storage)->mode != amd64::StorageMode::DirectRegister)
							continue;
						if(((amd64::VariableStorage*)dst->storage)->reg == storage->reg)
						{
							#if false
								code->placeSymbol(SymbolType::CodeLocation,0,".debug: moving arg "+src->name+" to avoid parameter "+dst->name);
								code->push({amd64::opcode::nop});
							#endif
							args[isrc] = new variable(*src);
							args[isrc]->storage = (void*)(new amd64::VariableStorage);
							((amd64::VariableStorage*)args[isrc]->storage)->displacement = ImmediateValue(negative(func->stack->push(src->dataType->size)));
							((amd64::VariableStorage*)args[isrc]->storage)->mode = amd64::StorageMode::IndirectRegister;
							((amd64::VariableStorage*)args[isrc]->storage)->reg = amd64::Register::rbp;
							runtime::copy(src,args[isrc]);
							moved = true;
						}
					}
					if(!moved)
					{
						variable* save = new variable(*src);
						((amd64::VariableStorage*)save->storage)->displacement = ImmediateValue(negative(func->stack->push(src->dataType->size)));
						((amd64::VariableStorage*)save->storage)->mode = amd64::StorageMode::IndirectRegister;
						((amd64::VariableStorage*)save->storage)->reg = amd64::Register::rbp;
						saved.push_back(save);
					}
					else
					{
						saved.push_back(nullptr);
					}
				}
				for(uint64_t i = 0;i<args.size();i++)
				{
					variable* src = args[i];
					variable* dst = func->vparams[i];
					#if false
						code->placeSymbol(SymbolType::CodeLocation,0,".debug: copy "+src->name+" to parameter "+dst->name);
					#endif
					runtime::copy(src,dst);
				}
				runtime::call(func);
				for(uint64_t i = 0;i<args.size();i++)
				{
					if(saved[i] == nullptr)
						continue;
					#if false
						code->placeSymbol(SymbolType::CodeLocation,0,".debug: restore "+args[i]->name);
					#endif
					runtime::copy(saved[i],args[i]);
				}
				func->stack->pop(pushed);
				return func->returnValue;
			}
			default:
				compilerBug("call not implemented for current architecture");
		}
	}
	compilerBug("call failure");
	return nullptr;
}

line compLine(std::string text)
{
	line l;
	l.file = "code generated by compiler";
	l.leadingSpaces = 0;
	l.lineNum = 0;
	l.text = text;
	return l;
}

struct targtype 
{
	std::string tname = "";
	type* rtype = nullptr;
};
std::vector<targtype*> targTypes;

std::stack<bool> isTemplateInstance;

/**
 * @brief Create a Type Template Instance
 * 
 * @callgraph
 * @callergraph
 * 
 * @param instanceString 
 * @param tt 
 * @param argstring 
 * @return type*
 */
type* createTypeTemplateInstance(std::string instanceString,typeTemplate* tt,std::string argstring)
{
	isTemplateInstance.push(true);
	if(options::ddebug)
		std::cout << "creating template instance \"" << instanceString << "\"" << std::endl;
	//fetch argument expressions and resolve them
	argstring.push_back(',');
	std::string working = "";
	uint64_t i = 0;
	for(char c : argstring)
	{
		switch(c)
		{
			case(0x00):
				goto ctti_endText;
			case(','):
			{
				line L;
				L.lineNum = 0;
				L.leadingSpaces = 0;
				L.text = working;
				token TAT = L.nextToken();
				//if(TAT.text.back() = '>')
				//    TAT.text.pop_back();
				if(tt->tArgs[i]->Type == 1)
				{
					if(options::ddebug)
						std::cout << "resolving integer template argument: \""+working+"\"" << std::endl;
					variable* RTA = resolve(TAT);
					compilerBug("unimplemented: RTA validity check");
					RTA->name = tt->tArgs[i]->name;
					//tempVariables.push_back(RTA);
					if(options::ddebug)std::cout << "template argument resolved!" << std::endl;
				}
				else if(tt->tArgs[i]->Type == 2)
				{
					targtype* tat = new targtype;
					if(options::ddebug)
						std::cout << "resolving typename template argument: \""+working+"\"" << std::endl;
					type* rtype = getType(working);
					tat->rtype = rtype;
					tat->tname = tt->tArgs[i]->name;
					targTypes.push_back(tat);
					if(options::ddebug)std::cout << "template argument resolved!" << std::endl;
				}
				else if(tt->tArgs[i]->Type == 3)
					compilerBug("tt->tArgs[i]->Type == 3, missing implentation.",originCoreHere,source(),"");
				//prepare for next argument
				working = "";
				i++;
				break;
			}
			default:
				working.push_back(c);
		}
	}
	ctti_endText:;
	//create template instance type
	type* tti = new type;
	tti->name = instanceString;
	tti->regMode = 0;
	tti->size	 = 0;
	defaultMangler->mangle(tti);
	std::vector<line> code;
	//default constructor function
	function* ctor = new function;
	tti->ctor = ctor;
	ctor->abi = defaultABI;
	types.push_back(tti);//push tti prematurely in order to be able to fetch pointer type
	type* tti_pointer = getType(tti->name+"&");
	ctor->parameters = {tti_pointer};
	variable* arg_this = new variable;
	arg_this->dataType = tti_pointer;
	arg_this->name = "this";
	ctor->vparams = {arg_this};
	compilerBug("unimplemented: this storage");
	ctor->isLocal = true;
	ctor->doExport = false;
	ctor->returnType = getType("void");
	ctor->name = "__c2_constructor_"+tti->mangledName;
	if(tt->sc->name == "global")
		ctor->symbol = ctor->name;
	else
		ctor->symbol = tt->sc->name+CPE2_SYMBOL_SCOPE_SEP+ctor->name;
	//scope
	scope* ttiScope = new scope;
	ttiScope->t = scopeType::CLASS;
	ttiScope->cl = tti;
	ttiScope->func = ctor;
	ttiScope->isIndentBased = true;
	ttiScope->parent = tt->sc;
	ttiScope->name = tti->mangledName;
	scope* trueCurrentScope = currentScope;
	Entity::updateCurrentScope(ttiScope);
	//vstcDisableSend = true;
	parse::Lines(tt->code);
	//vstcDisableSend = false;
	tti->incomplete = false;
	//finish up constructor
	{
		section* fcode = new section;
		if(!ctor->isLocal)
			compilerBug("unimplemented: create global symbol");
		//finish up function
		ctor->abi->genProlouge(fcode, ttiScope);
		fcode->push(ctor->code);
		ctor->abi->genEpilouge(fcode, ttiScope);
		for(section* block : ttiScope->extraCodeBlocks)
			fcode->push(block);
		compilerBug("unimplemented: add function code to output");
	}
	targTypes.clear();
	Entity::updateCurrentScope(trueCurrentScope);
	if(options::ddebug)std::cout << "template type created" << std::endl;
	isTemplateInstance.pop();
	return tti;
}

/**
 * @brief fetch a type by its name
 * 
 * @callgraph
 * @callergraph
 * 
 * @param name 
 * @return type*, throws an exception if the type cannot be found
 */
type* getType(std::string name) {
	if(options::ddebug)
		std::cout << "fetching type: \"" << name << "\"" << std::endl;
	if(name == "operator*")
		noSuchType("",originCoreHere,source(),name);
	for(targtype* i : targTypes)
	{
		if(name == i->tname)
		{
			//std::cout << "substitued template argument type: " << i->rtype->name << " for " << name << std::endl;
			name = i->rtype->name;
			break;
		}
		else if(name.substr(0,name.length()-1) == i->tname)
		{
			//std::cout << "substitued template argument type: " << i->rtype->name << " for " << name.substr(0,name.length()-1) << std::endl;
			name = i->rtype->name+name.back();
			break;
		}
		//else
		//    std::cout << "\"" << name << "\" != \"" << i->tname << "\"" << std::endl;
	}
	{//TODO: remove this later, this is just for debugging purposes
		if(name.find_first_of('*') != std::string::npos && name.length()-name.find_first_of('*') >= 10)
		{
			exit(-1);
		}
	}
	for (type* t : types)
		if (t->name == name) return t;
	if (name.back() == '*') {
		// unknown pointer type, create a new type
		//std::cout << "creating pointer type: \"" << name << "\"" << std::endl;
		type* t = new type;
		t->name = name;
		SETBIT_00(t->miscData);//mark as pointer type
		std::string vtn = name.substr(0,name.length()-1);
		t->valueType = getType(vtn);
		t->__declared_file = t->valueType->__declared_file;
		t->__declared_line = t->valueType->__declared_line;
		t->size = POINTER_SIZE;
		defaultMangler->mangle(t);
		types.push_back(t);
		std::vector<line> lines;
		lines.push_back(compLine("primitiveAssign primitiveInPlace void operator=(ptr_t,"+name+");"));
		lines.push_back(compLine("primitiveAssign primitiveInPlace void operator=("+name+",ptr_t);"));
		lines.push_back(compLine("primitiveAssign primitiveInPlace void operator=("+name+",u64);"));
		lines.push_back(compLine("primitiveAssign primitiveInPlace void operator=("+name+","+name+");"));
		lines.push_back(compLine("primitiveAdd primitiveInPlace void operator+=("+name+",u64);"));
		lines.push_back(compLine("primitiveAdd primitiveInPlace void operator+=("+name+","+name+");"));
		lines.push_back(compLine("primitiveSub primitiveInPlace void operator-=("+name+",u64);"));
		lines.push_back(compLine("primitiveSub primitiveInPlace void operator-=("+name+","+name+");"));
		lines.push_back(compLine("primitiveMul primitiveInPlace void operator*=("+name+",u64);"));
		lines.push_back(compLine("primitiveMul primitiveInPlace void operator*=("+name+","+name+");"));
		lines.push_back(compLine("primitiveDiv primitiveInPlace void operator/=("+name+",u64);"));
		lines.push_back(compLine("primitiveDiv primitiveInPlace void operator/=("+name+","+name+");"));
		lines.push_back(compLine("primitiveMod primitiveInPlace void operator%=("+name+",u64);"));
		lines.push_back(compLine("primitiveMod primitiveInPlace void operator%=("+name+","+name+");"));
		lines.push_back(compLine("primitiveArrayIndex primitiveInPlace "+t->valueType->name+" operator[]("+name+",u64);"));
		parse::Lines(lines,"compiler-generated-code");
		return t;
	}
	if (name.back() == '&') {
		type* t = new type;
		t->name = name;
		SETBIT_01(t->miscData);//mark as reference type
		std::string vtn = name.substr(0,name.length()-1);
		t->valueType = getType(vtn);
		if(t->valueType == nullptr)
		{
			//value type does not exist
			std::cerr << "\033[31mERROR:\033[0m \"" << name.substr(0,name.length()-1) << "\" does not name a type!" << std::endl;
			return nullptr;
		}
		t->size = POINTER_SIZE;
		defaultMangler->mangle(t);
		types.push_back(t);
		return t;
	}
	{//check for template
		uint64_t tname_start = name.find_first_of('<');
		if(tname_start != std::string::npos)
		{
			std::string tname = name.substr(0,tname_start);
			for(typeTemplate* i : typeTemplates)
			{
				if(i->name == tname)
				{
					type* tti = createTypeTemplateInstance(name,i,name.substr(tname_start+1,name.find_last_of('>')-1));
					return tti;
				}
			}
		}
	}
	noSuchType("",originCoreHere,source(),name);
	return nullptr;
}
ABI* getABI(std::string name)
{
	for(ABI* abi : ABIs)
		if(abi->name == name)
			return abi;
	noSuchABI("",originCoreHere,source(),name);
	return nullptr;
}

std::string getIndent() {
	scope* sc = currentScope;
	std::string ret = "";
	while (sc != nullptr) {
		if(sc->t == scopeType::CONDITIONAL_BLOCK || sc->t == scopeType::DUMMY || sc->t == scopeType::LOGICAL || sc->t == scopeType::TRY || sc->t == scopeType::TRY_CATCH)
		{
			ret += "\t";
		}
		else if(sc->t == scopeType::CATCH || sc->t == scopeType::FUNCTION)
		{
			ret += "\t";
			break;
		}
		sc = sc->parent;
	}
	return ret;
}

static std::string getTokenTypename(uint64_t t)
{
    switch(t)
    {
        case(12):
        case(9):
            return "typename";
        case(2):
            return "immediate";
        case(3):
            return "operator";
        case(6):
        case(7):
            return "string literal";
        case(8):
            return "keyword";
        case(10):
            return "variable name";
		case(11):
            return "function name";
        case(1):
            return "new unique identifier";
        case(30):
            return "opening round bracket";
        case(31):
            return "closing round bracket";
        case(32):
            return "opening sqaure bracket";
        case(33):
            return "closing square bracket";
        case(34):
            return "opening angle bracket";
        case(35):
            return "closing angle bracket";
        case(36):
            return "opening curly bracket";
        case(37):
            return "closing curly bracket";
        case(40):
            return "colon";
        case(41):
            return "semicolon";
        case(42):
            return "comma";
    }
    return "INVALID TOKEN TYPE ("+std::to_string(t)+")";
}

function* getFunction(std::string name)
{ 
	scope* sc = currentScope;
	while (sc != nullptr) {
		for (function* i : sc->functions) 
			if (i->name == name) 
				return i;
		sc = sc->parent;
	}
	return nullptr;
}

_system* getSystem(std::string name)
{
	for(_system* i : systems)
		if(i->name == name)
			return i;
	noSuchSystem("",originCoreHere,source(),name);
	return nullptr;
}

/**
 * @brief Get the Print Function Expression object
 * 
 * @callgraph
 * @callergraph
 * 
 * @param f 
 * @param showVariableNames 
 * @return std::string 
 */
std::string getPrintFunctionExpression(function* f, bool showVariableNames) {
	std::string res = COLOR_TYPE + f->returnType->name + " "+ COLOR_FUNCTION + f->name +COLOR_RESET+ "(";
	if (!showVariableNames) {
		for (type* i : f->parameters)
			res += COLOR_TYPE + i->name +COLOR_RESET+ ",";
	} else {
		for (variable* i : f->vparams)
		{
			res += COLOR_TYPE + i->dataType->name + " " +COLOR_VAR+ i->name +COLOR_RESET+ ",";
		}
	}
	if (res.back() == ',') res.pop_back();
	res += ")";
	return res;
}

/**
 * @brief Get the Function Expression object
 * 
 * @callgraph
 * @callergraph
 * 
 * @param f 
 * @param showVariableNames 
 * @return std::string 
 */
std::string getFunctionExpression(function* f, bool showVariableNames = false) {
	std::string res;
	if(f->returnType != nullptr)
		res += f->returnType->name + " " + f->name + "(";
	else
		res += f->name + "(";
	if (!showVariableNames) {
		for (type* i : f->parameters)
			res += i->name + ",";
	} else {
		for (variable* i : f->vparams)
		{
			res += i->dataType->name + " " + i->name + ",";
		}
	}
	if (res.back() == ',') res.pop_back();
	res += ")";
	return res;
}

#include <stacktrace.hxx>
/**
 * @brief
 * 
 * @callgraph
 * @callergraph
 * 
 * @param name 
 * @param args 
 * @return std::string 
 */
std::string getFunctionExpression(std::string name,
								  std::vector<variable*>& args) {
	std::string res = name + "(";
	for (variable* i : args)
	{
		if(i == nullptr)
			compilerBug("(variable* i : args) == nullptr",originCoreHere,source(),"");
		if(i->dataType == nullptr)
			compilerBug("(variable* i : args)->dataType == nullptr",originCoreHere,source(),"");
		res += i->dataType->name + ",";
	}
	if (res.back() == ',') 
		res.pop_back();
	res += ")";
	return res;
}

/**
 * @brief Get the Function object
 * 
 * @callgraph
 * @callergraph
 * 
 * @deprecated
 * 
 * @param name 
 * @param args 
 * @return function* 
 */
function* getFunction(const char* name, std::vector<variable*>& args)
{
	std::string n = name;
	return getFunction(n,args);
}

/**
 * @brief Get the Function object
 * 
 * @callgraph
 * @callergraph
 * 
 * @param name 
 * @param args 
 * @return function* 
 */
function* getFunction(std::string& name, std::vector<variable*>& args) {
	scope* sc = currentScope;
	std::vector<function*> candidates;
	while (sc != nullptr) {
		for (function* i : sc->functions) {
			//PRINT_DEBUG
			if (i->name == name) {
				//PRINT_DEBUG
				if (i->vparams.size() == args.size()) {
					//PRINT_DEBUG
					for (uint64_t II = 0; II < i->parameters.size(); II++){
						//PRINT_DEBUG
						//std::cout << "name: " << name << std::endl;
						//std::cout << "II: " << II << std::endl;
						//std::cout << "args size: " << args.size() << std::endl;
						//std::cout << "params size: " << i->parameters.size() << std::endl;
						//std::cout << "aII: " <<std::hex << (void*)args[II] << std::endl;
						//std::cout << "aII->dt: " <<std::hex << (void*)args[II]->dataType << std::endl;
						if (i->parameters[II] != args[II]->dataType)
							goto notThisFunction;
					}
					// else
					//     std::cout <<std::hex<<i->parameters[II]<< " != " <<
					//     args[II] << std::endl;
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
	function* func = new function;
	func->name = name;
	func->vparams = args;
	for(variable* v : func->vparams)
		func->parameters.push_back(v->dataType);
	func->returnType = anyType;
	noSuchFunction("",originCoreHere,source(),func,candidates);
	return nullptr;
}

/**
 * @brief Get the Function object
 * 
 * @callgraph
 * @callergraph
 * 
 * @param returnType 
 * @param name 
 * @param args 
 * @return function* 
 */
function* getFunction(type* returnType, std::string& name, std::vector<variable*>& args) {
	scope* sc = currentScope;
	std::vector<function*> candidates;
	while (sc != nullptr) {
		for (function* i : sc->functions) {
			if((i->returnType == returnType) || (returnType == anyType))
			{
				//PRINT_DEBUG
				if (i->name == name) {
					//PRINT_DEBUG
					if (i->vparams.size() == args.size()) {
						//PRINT_DEBUG
						for (uint64_t II = 0; II < i->parameters.size(); II++){
							//PRINT_DEBUG
							//std::cout << "name: " << name << std::endl;
							//std::cout << "II: " << II << std::endl;
							//std::cout << "args size: " << args.size() << std::endl;
							//std::cout << "params size: " << i->parameters.size() << std::endl;
							//std::cout << "aII: " <<std::hex << (void*)args[II] << std::endl;
							//std::cout << "aII->dt: " <<std::hex << (void*)args[II]->dataType << std::endl;
							if (i->parameters[II] != args[II]->dataType)
								goto notThisFunction;
						}
						// else
						//     std::cout <<std::hex<<i->parameters[II]<< " != " <<
						//     args[II] << std::endl;
						return i;
					}
				}
				notThisFunction:;
					candidates.push_back(i);
			}
			//else
			//    std::cout << i->name <<" != "<< name << std::endl;
		}
		sc = sc->parent;
	}
	function* func = new function;
	func->name = name;
	func->vparams = args;
	func->returnType = returnType;
	noSuchFunction("",originCoreHere,source(),func,candidates);
	return nullptr;
}

std::vector<variable*> tempVariables;
/**
 * @brief Get the Variable object
 * 
 * @callgraph
 * @callergraph
 * 
 * @param name 
 * @return variable* 
 */
variable* getVariable(std::string name) {
	scope* sc = currentScope;
	while (sc != nullptr) {
		//std::cout << "scanning scope: " << sc->name << std::endl;
		for (variable* i : sc->variables)
			if (i->name == name) return i;
			//else std::cout << "\"" << i->name << "\" != \"" << name << "\"" << std::endl;
		sc = sc->parent;
	}
	for(variable* i : tempVariables)
	{
		if(i->name == name) return i;
	}
	noSuchVariable("",originCoreHere,source(),name);
	return nullptr;
}
