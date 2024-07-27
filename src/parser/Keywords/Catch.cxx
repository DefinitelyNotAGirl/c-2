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
		uint64_t save_routine_offset;
		uint64_t save_datadst_offset;
		uint64_t datadst_offset;
		std::string symbol_routine;
		std::string symbol_threaddata_routine;
		std::string symbol_threaddata_datadst;
		std::string reentry;
	};
	RoutineData_T* RoutineData = new RoutineData_T;
	RoutineData->catchType = catchType;
	RoutineData->sc = sc;
	RoutineData->save_routine_offset = currentScope->func->stack->push(8);
	RoutineData->save_datadst_offset = currentScope->func->stack->push(8);
	RoutineData->datadst_offset = currentScope->func->stack->push(catchType->size);
	RoutineData->symbol_routine = sc->name;
	RoutineData->symbol_threaddata_routine = symbol_threaddata_routine;
	RoutineData->symbol_threaddata_datadst = symbol_threaddata_datadst;
	RoutineData->reentry = currentScope->name+CPE2_SYMBOL_SCOPE_SEP+"reentry";
	//,
	//, pre code
	//,
	currentScope->Prologue.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			//,
			//, save old handler
			//,
			{
				//. load old handler address to rax
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
					0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_routine
						)
					);
				}
				//. save rax to stack
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::rbp)
				});
				code->push(amd64::imm32(data->save_routine_offset));
				//. load old datadst to rax
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
					0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_datadst
						)
					);
				}
				//. save rax to stack
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::rbp)
				});
				code->push(amd64::imm32(data->save_datadst_offset));
			}
			//,
			//, install new handler
			//,
			{
				//. load handler address to rax
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__imm16_32_64 + amd64::register_decode_base(amd64::Register::rax),
					0,0,0,0,0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-8,
							8,
							smu::RelocationType::Absolute,
							data->symbol_routine
						)
					);
				}
				//.
				//. set routine address
				//.
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
					0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_routine
						)
					);
				}
				//.
				//. set data destination address
				//.
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rbp,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
					0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_datadst
						)
					);
				}
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::sub::rm16_32_64__imm16_32,
					amd64::modRM(5,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
				});
				code->push(amd64::imm32(data->datadst_offset));
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_datadst
						)
					);
				}
			}
		}
	));
	//,
	//, post code
	//,
	currentScope->Epilogue.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			//+
			//+ restore old handler
			//+
			{
				//. load old handler address
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::rbp)
				});
				code->push(amd64::imm32(data->save_routine_offset));
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
					0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_routine
						)
					);
				}
				//. load old datadst
				code->push({
					amd64::prefix::REX(1,0,0,0),
					amd64::opcode::mov::r16_32_64__rm16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::rbp)
				});
				code->push(amd64::imm32(data->save_datadst_offset));
				code->push({
					amd64::prefix::REX(1,0,0,1),
					amd64::opcode::mov::rm16_32_64__r16_32_64,
					amd64::modRM(amd64::Register::rax,amd64::AddressingMode::RegisterIndirect_disp32,amd64::Register::r15),
					0,0,0,0
				});
				//+
				//+ linker info
				//+
				{
					code->Relocations.push_back(
						smu::RelocationEntry(
							code->size()-4,
							4,
							smu::RelocationType::Absolute,
							data->symbol_threaddata_datadst
						)
					);
				}
			}
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
	//set return symbol
	Entity::updateCurrentScope(sc);
	//,
	//, add resource code
	//,
	if(catchType->ExceptionOffset == 0){
		resourceCode.push_back("c2resource exception "+catchType->mangledName+";");
	}
	catchType->ExceptionOffset = 1;
}
