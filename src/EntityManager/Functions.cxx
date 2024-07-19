#include <EntityManagement.hxx>
#include <Parser.hxx>
#include <output.hxx>

using Entity::Attribute;
using Entity::AttributeType;
using Entity::PrimitiveAttributeData;

using namespace issues;

static function* constructFunction(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType)
{
	if(returnType == nullptr)
	{
		std::cout << "blub blub" << std::endl;
	}
	mangler* NameMangler = defaultMangler;

	bool ImplicitCast = false;

	function* func = new function;
	func->code = new section;
	func->name = name;
	func->vparams = args;
	func->returnType = returnType;
	func->returnValue = new variable;
	func->returnValue->dataType = returnType;
	func->returnValue->name = "____cpe2returnvalue";
	func->abi = defaultABI;
	for(Attribute& attr : attributes)
	{
		switch(attr.Type)
		{
			case(AttributeType::Typecast):
			{
				SETBIT_00(func->miscData);//set cast bit
				if(func->vparams.size() == 1)
				{
					castFunction* ncf = new castFunction;
					ncf->input = func->vparams[0]->dataType;
					ncf->output = func->returnType;
					ncf->func = func;
					ncf->canImplicitCast = ImplicitCast;
					castFunctions.push_back(ncf);
				}
				else
				{
					std::cout << "invalid typecast function: " << func->expression_ansi() << std::endl;
					compilerBug("typecast function must take exactly 1 argument.",originCoreHere,source(),"");
				}
				break;
			}
			case(AttributeType::ExplicitCast):
			{
				ImplicitCast = false;
				break;
			}
			case(AttributeType::ImplicitCast):
			{
				ImplicitCast = true;
				break;
			}
			case(AttributeType::ABI):
			{
				func->abi = attr.Abi;
				break;
			}
			case(AttributeType::Deprecated):
			{
				func->isDeprecated = true;
				break;
			}
			case(AttributeType::Primitive):
			{
				func->isPrimitive = true;
				func->op = attr.Primitive.Operation;
				func->primitiveInPlace = attr.Primitive.InPlace;
				break;
			}
			case(AttributeType::Local):
			{
				func->isLocal = true;
				break;
			}
			case(AttributeType::Extern):
			{
				func->isExtern = true;
				break;
			}
			case(AttributeType::Mangling):
			{
				NameMangler = attr.Mangler;
				break;
			}
			case(AttributeType::Noreturn):
			{
				func->noReturn = true;
				break;
			}
			case(AttributeType::Symbol):
			{
				func->symbol = attr.Symbol;
				break;
			}
			default:
				invalidAttribute(
					"",originCoreHere,source(ParserState.File,*attr.Token.Line,attr.Token),
					"function",attr.Token.text,
					{
						"primitiveInPlace",
						"primitiveFloat",
						"primitiveAdd",
						"primitiveSub",
						"primitiveMul",
						"primitiveDiv",
						"primitiveMod",
						"primitiveEqual",
						"primitiveNotEqual",
						"primitiveGreater",
						"primitiveGreaterEqual",
						"primitiveLess",
						"primitiveLessEqual",
						"primitiveAnd",
						"primitiveXor",
						"primitiveOr",
						"primitiveNot",
						"primitiveInc",
						"primitiveDec",
						"primitiveAssign",
						"primitiveArrayIndex",
						"primitiveInterrupt",
						"primitiveCPUID",
						"primitiveSYSCALL",
						"primitivePRINTCHAR",
						"primitivePRINTSTR",
						"local",
						"public",
						"protected",
						"private",
						"inline",
						"const",
						"extern",
						"noop",
						"typecast",
						"implicitcast",
						"explicitcast",
						"noreturn",
						"deprecated",
						"SYMBOL-...",
						"mangling-...",
						"ABI-..."
					}
				);
		}
	}
	if(func->symbol.empty())
	{
		if(name == "main" && options::mainAutoSym)
			func->symbol = "cpe2main";
		else
			NameMangler->mangle(func);
	}
	globalScope->functions.push_back(func);
	return func;
}

function* Entity::declareFunction(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType)
{
	function* func = constructFunction(attributes,name,args,returnType);
	if(!func->isPrimitive)
	{
		func->abi->setFunctionStorages(func);
		importExternalFunction(func->symbol);
	}
	return func;
}

function* Entity::startFunctionDefinition(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType, bool isIndentBased)
{
	function* func = constructFunction(attributes,name,args,returnType);
	scope* sc		  = new scope;
	sc->parent		  = currentScope;
	sc->name		  = func->symbol;
	sc->leadingSpace  = ParserState.Line.leadingSpaces + tabLength;
	sc->isIndentBased = isIndentBased;
	sc->t			  = scopeType::FUNCTION;
	sc->func		  = func;
	// declare variables for arguments
	func->abi->setFunctionStorages(func);
	for (variable* arg : args) {
		sc->variables.push_back(arg);
		bool isPointer = arg->dataType->name.back() == '*' ? true : false;
		type* childTargetDataType = isPointer ? arg->dataType->valueType : arg->dataType;
		std::string msep = isPointer ? "->" : ".";
		if(isPointer)
		{
			for(variable i : childTargetDataType->members)
			{
				//init
				variable* child = new variable(i);
				child->doExport = false;
				//name
				child->name = arg->name+msep+child->name;
				//storage
				compilerBug("unimplemented: child storage");
				//finish up
				child->parent = arg;
				arg->children.push_back(child);
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
				child->name = arg->name+msep+child->name;
				//storage
				compilerBug("unimplemented: child storage");
				//finish up
				child->parent = arg;
				arg->children.push_back(child);
				currentScope->variables.push_back(child);
				if(options::ddebug)
					std::cout << "declared child: " << child->name << std::endl;
			}
		}
	}
	updateCurrentScope(sc);
	//,
	//, debug info
	//,
	{
		unimplementedDebugInfo("function body");
	}
	if (options::ddebug)
		std::cout << "body started" << std::endl;
	return func;
}
