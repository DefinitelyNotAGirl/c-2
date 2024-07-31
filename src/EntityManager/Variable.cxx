#include <EntityManagement.hxx>
#include <Parser.hxx>
#include <output.hxx>
#include <dump.hxx>
#include <event.hxx>

using Entity::Attribute;
using Entity::AttributeType;
using Entity::PrimitiveAttributeData;

using namespace issues;

variable* Entity::defineVariable(std::vector<Attribute>& attributes,std::string& name, type* Type)
{
	mangler* NameMangler = defaultMangler;
	variable* var	  = new variable;
	var->dataType	  = Type;
	mangler* mangling = defaultMangler;
	var->name	= name;
	var->Access = vaccess::_public;
	for(Attribute& attr : attributes)
	{
		switch(attr.Type)
		{
			case(AttributeType::Local):
			{
				var->isStatic = true;
				break;
			}
			case(AttributeType::Extern):
			{
				var->isExtern = true;
				break;
			}
			case(AttributeType::Accessibility):
			{
				var->Access = attr.Accessibility;
				break;
			}
			case(AttributeType::Constexpr):
			{
				var->isConstexpr = true;
				break;
			}
			case(AttributeType::Mangling):
			{
				NameMangler = attr.Mangler;
				break;
			}
			case(AttributeType::Symbol):
			{
				var->symbol = attr.Symbol;
				break;
			}
			case(AttributeType::RegisterStorage):
			{
				var->storageArch = currentArchitecture;
				if(currentArchitecture == Architecture::AMD64) {
					amd64::VariableStorage* storage = new amd64::VariableStorage;
					var->storage = (void*)storage;
					storage->mode = amd64::StorageMode::DirectRegister;
					storage->reg = amd64::string_to_register(attr.Token.text);
				}
				break;
			}
			default:
				invalidAttribute(
					"",originCoreHere,source(ParserState.File,*attr.Token.Line,attr.Token),
					"variable",attr.Token.text,
					{
						"local",
						"public",
						"protected",
						"private",
						"const",
						"constexpr",
						"extern",
						"deprecated",
						"SYMBOL-...",
						"mangling-...",
					}
				);
		}
	}
	if(var->symbol.empty())
		mangling->mangle(var);
	if(var->isExtern)
	{
		var->storageArch = currentArchitecture;
		if(!(currentScope->t == scopeType::GLOBAL || currentScope->t == scopeType::NAMESPACE))
			nonGlobalExtern("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token));
		if(currentArchitecture == Architecture::AMD64)
		{
			amd64::VariableStorage* storage = new amd64::VariableStorage;
			var->storage = storage;
			storage->mode = amd64::StorageMode::IndirectImmediate;
			storage->immediate = ImmediateValue(var->symbol);
			importExternalVariable(var->symbol);
		}
		else
		{
			compilerBug("unimplemented: external variable");
		}
	}
	if(var->isConstexpr)
	{
		var->storageArch = Architecture::storage_IntegerImmediate;
	}
	if(var->storage == nullptr && !var->isExtern && !var->isConstexpr)
	{
		var->usedAutoStorage = true;
		var->storageArch = currentArchitecture;
		//+
		//+ AMD64 local variable
		//+
		if(currentArchitecture == Architecture::AMD64 && (currentScope->t == scopeType::FUNCTION  || currentScope->t == scopeType::LOGICAL || currentScope->t == scopeType::CONDITIONAL_BLOCK || currentScope->t == scopeType::TRY || currentScope->t == scopeType::CATCH))
		{
			var->storage = new amd64::VariableStorage;
			amd64::VariableStorage* storage = (amd64::VariableStorage*)var->storage;
			amd64::Register ireg = currentScope->func->cpu.amd64.getFreeRegister();
			amd64::Register freg = amd64::Register::xmm0;
			if(var->dataType->regMode == 1 && var->dataType->size <= 8 && ireg != amd64::Register::invalid)
			{
				//+ integer register storage
				storage->mode = amd64::StorageMode::DirectRegister;
				storage->reg = ireg;
			}
			else if(var->dataType->regMode == 2 && var->dataType->size <= 32 && freg != amd64::Register::invalid)
			{
				//+ floating point register storage
				compilerBug("unimplemented: floating point register storage");
			}
			else
			{
				//+ stack storage
				storage->mode = amd64::StorageMode::IndirectRegister;
				storage->reg = amd64::Register::rbp;
				uint64_t offset = currentScope->func->stack->push(var->dataType->size);
				storage->displacement = ImmediateValue(negative(offset));
			}
		}
		//+
		//+ AMD64 global variable
		//+
		else if(currentArchitecture == Architecture::AMD64 && (currentScope->t == scopeType::GLOBAL || currentScope->t == scopeType::NAMESPACE))
		{
			var->storage = new amd64::VariableStorage;
			amd64::VariableStorage* storage = (amd64::VariableStorage*)var->storage;
			//+ global memory storage
			storage->mode = amd64::StorageMode::IndirectImmediate;
			storage->immediate = ImmediateValue(var->symbol);
			data.placeSymbol(SymbolType::GlobalVariable,0,var->symbol);
		}
		//+
		//+ AMD64 class member variable
		//+
		else if(currentArchitecture == Architecture::AMD64 && (currentScope->t == scopeType::CLASS))
		{
			var->storageArch = Architecture::storage_member;
			var->storage = (void*)(currentScope->cl->size);
			currentScope->cl->size += var->dataType->size;
			currentScope->cl->members.push_back(*var);
		}
		else
		{
			compilerBug("unimplemented architecture-scope combination.");
		}
	}
	if(ParserState.DeclarationData.ArraySize != nullptr)
	{
		if(currentScope->t == scopeType::FUNCTION  || currentScope->t == scopeType::LOGICAL || currentScope->t == scopeType::CONDITIONAL_BLOCK || currentScope->t == scopeType::TRY || currentScope->t == scopeType::CATCH)
		{
			uint64_t offset = currentScope->func->stack->push(var->dataType->valueType->size*((uint64_t)ParserState.DeclarationData.ArraySize->storage));
		}
		else if(currentScope->t == scopeType::CLASS)
		{
			uint64_t classArrayBase = currentScope->cl->size;
			currentScope->cl->size+=(var->dataType->valueType->size*((uint64_t)ParserState.DeclarationData.ArraySize->storage));
		}
		delete ParserState.DeclarationData.ArraySize;
		ParserState.DeclarationData.ArraySize = nullptr;
	}
	if((var->storage != nullptr)&&(currentScope->t == scopeType::FUNCTION  || currentScope->t == scopeType::LOGICAL || currentScope->t == scopeType::CONDITIONAL_BLOCK || currentScope->t == scopeType::TRY || currentScope->t == scopeType::CATCH)) {
		if(var->storageArch == Architecture::AMD64)
		{
			amd64::VariableStorage* storage = (amd64::VariableStorage*)var->storage;
			if(storage->mode == amd64::StorageMode::DirectRegister)
			{
			}
		}
	}
	if (var->storageArch == Architecture::AMD64) {
		amd64::VariableStorage* storage = (amd64::VariableStorage*)var->storage;
		if(options::fcpl > amd64::register_decode_cpl(storage->reg) && storage->mode == amd64::StorageMode::DirectRegister)
			insufficientPrivilegeLevel("access register "+std::string(amd64::register_name(storage->reg)),originCoreHere,source());
		if(storage->reg == amd64::Register::rsp && storage->mode == amd64::StorageMode::DirectRegister)
			stackPointerStorage(var->name,originCoreHere,source());
		if(storage->mode == amd64::StorageMode::IndirectImmediate && !storage->immediate.isSymbol)
			absoluteMemoryStorage("",originCoreHere,source(),storage->immediate.imm64);
	}
	else if (var->storageArch == Architecture::storage_member) {
	}
	if (currentScope->t != scopeType::CLASS)
	{
		currentScope->variables.push_back(var);
		bool isPointer = var->dataType->name.back() == '*' ? true : false;
		type* childTargetDataType = isPointer ? var->dataType->valueType : var->dataType;
		//std::cout << "vdt: " << std::hex << (void*)var->dataType << std::endl;
		//std::cout << "vdtvt: " << std::hex << (void*)var->dataType->valueType << std::endl;
		//std::cout << "isPointer: " << isPointer << std::endl;
		//std::cout << "ctdt: " << std::hex << (void*)childTargetDataType << std::endl;
		if(childTargetDataType->members.size() > 0)
		{
			std::string msep = isPointer ? "->" : ".";
			if(isPointer)
			{
				for(variable i : childTargetDataType->members)
				{
					//init
					variable* child = new variable(i);
					child->doExport = false;
					//name
					child->name = var->name+msep+child->name;
					//storage
					//finish up
					child->parent = var;
					var->children.push_back(child);
					currentScope->variables.push_back(child);
					if(options::ddebug)
						std::cout << "declared child: " << child->name << std::endl;
				}
			}
			else
			{
				for(variable i : childTargetDataType->members)
				{
					//init
					variable* child = new variable(i);
					child->doExport = false;
					//name
					child->name = var->name+msep+child->name;
					//storage
					//finish upf
					child->parent = var;
					var->children.push_back(child);
					currentScope->variables.push_back(child);
					if(options::ddebug)
						std::cout << "declared child: " << child->name << std::endl;
				}
			}
		}
	}
	//dump("declared variable",var,"");
	//,
	//, debug info
	//,
	if(options::debugSymbols)
	{
		compilerBug("unimplemented: generate debug info");
	}
	Event::VariableDeclaration.fire((Event::Data::VariableDeclaration*)&var);
	return var;
}
