#include "../parser.hxx"

void parse::Keywords::Try()
{
	//,
	//, create TRY_CATCH scope
	//,
	{
		scope* sc = new scope;
		sc->leadingSpace=currentScope->leadingSpace+tabLength;
		sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"try_catch"+std::to_string(currentScope->tryCounter);
		sc->parent = currentScope;
		sc->t = scopeType::TRY_CATCH;
		sc->func = new function;
		//sdump(currentScope);
		*(sc->func) = *(currentScope->func);
		sc->func->code = new section;
		sc->extraCodeBlocks.push_back(new section);//pre code
		sc->extraCodeBlocks.push_back(new section);//post code
		sc->reentrySymbol = sc->name+CPE2_SYMBOL_SCOPE_SEP"epilogue";
		scope* acs = currentScope;
		currentScope = sc;
		struct RoutineData_T {
			scope* sc;
			std::string reentry;
		};
		RoutineData_T* RoutineData = new RoutineData_T;
		RoutineData->sc = sc;
		RoutineData->reentry = currentScope->name+CPE2_SYMBOL_SCOPE_SEP+"reentry";
		sc->StartClosure.push_back(Routine(RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				code = new section;
			}
		));
		//+
		//+ pre code
		//+
		{
			sc->Prologue.push_back(Routine(RoutineData,
				[](void* __data){
					RoutineData_T* data = (RoutineData_T*)__data;
					code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP"prologue");
				}
			));
		}
		//+
		//+ body code
		//+
		{
			sc->BodyCode.push_back(Routine(RoutineData,
				[](void* __data){
					RoutineData_T* data = (RoutineData_T*)__data;
					code->push(data->sc->func->code);
				}
			));
		}
		//+
		//+ post code
		//+
		{
			sc->Epilogue.push_back(Routine(RoutineData,
				[](void* __data){
					RoutineData_T* data = (RoutineData_T*)__data;
					code->placeSymbol(SymbolType::CodeLocation,0,data->reentry);
				}
			));
		}
		sc->Finalize.push_back(Routine(RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				data->sc->parent->func->code->push(code);
				for(Routine& r : data->sc->BranchCode)
					data->sc->parent->BranchCode.push_back(r);
			}
		));
		currentScope = acs;
		Entity::updateCurrentScope(sc);
	}
	//,
	//, create try scope
	//,
	{
		scope* sc = new scope;
		sc->leadingSpace=currentScope->leadingSpace;
		sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"try"+std::to_string(currentScope->tryCounter);
		sc->parent = currentScope;
		ParserState.Token = ParserState.Line.nextToken();
		if(ParserState.Token.type == 40 || ParserState.Token.type == 36)
		{
			sc->isIndentBased = ParserState.Token.type == 40;
			sc->parent->isIndentBased = sc->isIndentBased;
		}
		else
		{
			delete sc;
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{40,36});
		}
		sc->t = scopeType::TRY;
		sc->func = new function;
		*(sc->func) = *(currentScope->func);
		sc->func->code = new section;
		Entity::updateCurrentScope(sc);
		code->placeSymbol(SymbolType::CodeLocation,0,currentScope->name);
		struct RoutineData_T {
			scope* sc;
		};
		RoutineData_T* RoutineData = new RoutineData_T;{
			RoutineData->sc = sc;
		}
		sc->StartClosure.push_back(Routine(RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				code = new section;
			}
		));
		//+
		//+ pre code
		//+
		{
			sc->Prologue.push_back(Routine(RoutineData,
				[](void* __data){
					RoutineData_T* data = (RoutineData_T*)__data;
					code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP"prologue");
				}
			));
		}
		//+
		//+ body code
		//+
		{
			sc->BodyCode.push_back(Routine(RoutineData,
				[](void* __data){
					RoutineData_T* data = (RoutineData_T*)__data;
					code->push(data->sc->func->code);
				}
			));
		}
		//+
		//+ post code
		//+
		{
			sc->Epilogue.push_back(Routine(RoutineData,
				[](void* __data){
					RoutineData_T* data = (RoutineData_T*)__data;
					code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP"epilogue");
				}
			));
		}
		sc->Finalize.push_back(Routine(RoutineData,
			[](void* __data){
				RoutineData_T* data = (RoutineData_T*)__data;
				data->sc->parent->func->code->push(code);
				for(Routine& r : data->sc->BranchCode)
					data->sc->parent->BranchCode.push_back(r);
			}
		));
	}
	currentScope->tryCounter++;
}
