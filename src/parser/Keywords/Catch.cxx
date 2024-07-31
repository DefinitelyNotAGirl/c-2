#include "../parser.hxx"

#include <output.hxx>
#include <cgu.h>

void parse::Keywords::Catch() 
{
	if(options::ddebug)std::cout << "catch" << std::endl;
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"catch";
	sc->parent = currentScope;
	sc->t = scopeType::CATCH;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	section* preCode = new section;
	section* postCode = new section;
	//,
	//, get type to catch
	//,
	type* catchType;
	std::string symbol_threaddata_routine;
	std::string symbol_threaddata_datadst;
	bool useRoundBrackets = false;
	{
		ParserState.Token = ParserState.Line.nextToken();
		if(ParserState.Token.type == 30)
		{
			useRoundBrackets = true;
			ParserState.Token = ParserState.Line.nextToken();
		}
		if(ParserState.Token.type != 9)
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{9});
		catchType = getType(ParserState.Token.text);
		symbol_threaddata_routine = std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"exceptions"+CPE2_SYMBOL_SCOPE_SEP+catchType->mangledName+CPE2_SYMBOL_SCOPE_SEP+"routine";
		symbol_threaddata_datadst = std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"exceptions"+CPE2_SYMBOL_SCOPE_SEP+catchType->mangledName+CPE2_SYMBOL_SCOPE_SEP+"datadst";
		importExternalValue(symbol_threaddata_routine);
		importExternalValue(symbol_threaddata_datadst);
	}
	sc->name += CPE2_SYMBOL_SCOPE_SEP + catchType->mangledName;
	//,
	//, declare variable for exception
	//,
	variable* var;
	{
		ParserState.Token = ParserState.Line.nextToken();
		if(ParserState.Token.type != 1)
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{1});
		var = new variable;
		var->dataType = catchType;
		var->storageArch = currentArchitecture;
		if(currentArchitecture == Architecture::AMD64)
		{
			amd64::VariableStorage* storage = new amd64::VariableStorage;
			var->storage = (void*)storage;
			storage->mode = amd64::StorageMode::IndirectRegister;
			storage->reg = amd64::Register::rbp;
			storage->displacement = ImmediateValue(negative(currentScope->func->stack->push(catchType->size)));
		}
		else
			compilerBug("unsupported current architecture: "+std::to_string((uint64_t)currentArchitecture));
		var->name = ParserState.Token.text;
		sc->variables.push_back(var);
	}
	//,
	//,	check body type
	//,
	{
		ParserState.Token = ParserState.Line.nextToken();
		if(ParserState.Token.type == 31 && useRoundBrackets)
		{
			//. closing round bracket
			ParserState.Token = ParserState.Line.nextToken();
		}
		if(ParserState.Token.type == 40)
			sc->isIndentBased = true;
		else if(ParserState.Token.type == 36)
			sc->isIndentBased = false;
		else
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{40,36});
	}
	struct RoutineData_T {
		type* catchType;
		scope* sc;
		std::string reentry;
		//std::string RestoreExceptionHandler;
	};
	RoutineData_T* RoutineData = new RoutineData_T;
	RoutineData->catchType = catchType;
	RoutineData->sc = sc;
	RoutineData->reentry = currentScope->name+CPE2_SYMBOL_SCOPE_SEP+"reentry";
	//RoutineData->RestoreExceptionHandler = Entity::ExceptionHandlers.top();
	currentScope->ExceptionHandlerCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->push({
				amd64::opcode::cmp::rAX__imm16_32,
				0,0,0,0
			});
			code->Relocations.push_back(
				smu::RelocationEntry(
					code->size()-4,
					4,
					smu::RelocationType::Absolute,
					data->catchType->getRuntimeTypeID()
				)
			);
			code->push({
				0x0F,
				::amd64::opcode::secondary::jcc::rel16_32off(::amd64::Condition::Equal),
				0,0,0,0
			});
			code->Relocations.push_back(
				smu::RelocationEntry(
					code->size()-4,
					4,
					smu::RelocationType::Relative,
					data->sc->name
				)
			);
		}
	));
	//,
	//, handler code
	//,
	currentScope->BranchCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name);
			code->push(data->sc->func->code);
			runtime::RelativeControlTransfer(data->reentry);
		}
	));
	sc->Finalize.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			//Entity::ExceptionHandlers.push(data->RestoreExceptionHandler);
		}
	));
	//set return symbol
	Entity::updateCurrentScope(sc);
	//Entity::ExceptionHandlers.pop();
}
