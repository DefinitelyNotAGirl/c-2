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
		sc->isIndentBased = true;
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
		struct OnCloseData_T {
			scope* sc;
		};
		OnCloseData_T* OnCloseData = new OnCloseData_T;
		OnCloseData->sc = sc;
		//+
		//+ pre code
		//+
		{
			sc->Prologue.push_back(Routine(OnCloseData,
				[](void* __data){
					OnCloseData_T* data = (OnCloseData_T*)__data;
					code->placeSymbol(SymbolType::CodeLocation,0,data->sc->name+CPE2_SYMBOL_SCOPE_SEP"prologue");
				}
			));
			ParserState.trycatchSaveallBase.push(runtime::SaveAll());
		}
		//+
		//+ post code
		//+
		{
			sc->Epilogue.push_back(Routine(OnCloseData,
				[](void* __data){
					OnCloseData_T* data = (OnCloseData_T*)__data;
					code->placeSymbol(SymbolType::CodeLocation,0,data->sc->reentrySymbol);	
					delete data;
				}
			));
		}
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
		if(ParserState.Token.type == 40)
		{
			sc->isIndentBased = true;
		}
		else if(ParserState.Token.type == 36)
		{
			sc->isIndentBased = false;
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

	}
	currentScope->tryCounter++;
}
