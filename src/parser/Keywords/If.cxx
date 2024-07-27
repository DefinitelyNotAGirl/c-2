#include "../parser.hxx"

using namespace issues;

void parse::Keywords::If()
{
	if(currentScope->t == scopeType::GLOBAL)
		invalidUseOfKeywordInScope("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),currentScope);
	if(options::ddebug)std::cout << "if" << std::endl;
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"if"+std::to_string(currentScope->ifCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = true;
	sc->t = scopeType::CONDITIONAL_BLOCK;
	sc->func = new function;
	sc->parent->conditionalCounter++;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	sc->reentrySymbol = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"conditional"+std::to_string(sc->parent->conditionalCounter)+CPE2_SYMBOL_SCOPE_SEP"reentry";
	sc->extraCodeBlocks.push_back(sc->func->code);
	//generate conditional jump code
	line cl = ParserState.Line;
	cl.text = "";
	cl.tpos = 0;
	token cond;
	if(ParserState.Token.type == 30)
	{
		//PRINT_DEBUG
		ParserState.Token = ParserState.Line.nextToken();
		cond = ParserState.Token;
		//while(cond.type != 31)//while != )
		//{
		//    //std::cout << "type: " << ParserState.Token.type << std::endl;
		//    //std::cout << "text: " << 
		//    cond = ParserState.Line.nextToken();
		//    cl.text += cond.text;
		//}
		//cl.tpos = 0;
		for(char c : ParserState.Line.restText())
		{
			switch(c)
			{
				case(')'):
					goto endCLine0;
				default:
					cl.text.push_back(c);
			}
		}
		endCLine0:;
	}
	else
	{
		//PRINT_DEBUG
		cond = ParserState.Token;
		uint64_t otpos = ParserState.Line.tpos;
		while(cond.type != 40)//while != :
		{
			//PRINT_DEBUG
			cond = ParserState.Line.nextToken();
			//if(cond.type != 40) {
			//    cl.text += cond.text;
			//}
		}
		//cl.tpos = 0;
		ParserState.Line.tpos = otpos;
		for(char c : ParserState.Line.restText())
		{
			//PRINT_DEBUG
			switch(c)
			{
				case(':'):
					goto endCLine1;
				default:
					cl.text.push_back(c);
			}
		}
		endCLine1:;
	}
	cond = cl.nextToken();
	variable* condition = resolve(cond);
	compilerBug("unimplemented: if, conditional jump");
	struct RoutineData_T {
		scope* sc;
	};
	RoutineData_T* RoutineData = new RoutineData_T;
	RoutineData->sc = sc;
	Entity::updateCurrentScope(sc);
	currentScope->BranchCode.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			code->push(data->sc->func->code);
		}
	));
	currentScope->Finalize.push_back(Routine(RoutineData,
		[](void* __data){
			RoutineData_T* data = (RoutineData_T*)__data;
			for(Routine& r : data->sc->BranchCode)
				data->sc->parent->BranchCode.push_back(r);
		}
	));
	compilerBug("unimplemented: if, set return symbol");
}
