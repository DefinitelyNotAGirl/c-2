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
	//std::cout << "transfer " << currentScope->name << " -> " << sc->name << std::endl;
	*(sc->func) = *(currentScope->func);
	//std::cout << "parent stack offset: " <<std::dec<< sc->fstore->stackOffset << std::endl;
	//std::cout << "parent func stack offset: " <<std::dec<< sc->func->fstore->stackOffset << std::endl;
	//std::cout << "parent stack size: " <<std::dec<< sc->fstore->stackSize << std::endl;
	//std::cout << "parent func stack size: " <<std::dec<< sc->func->fstore->stackSize << std::endl;
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
	//std::cout << "condition: " << cl.text << std::endl;
	variable* condition = resolve(cond);
	//cmp(__false__,condition);
	compilerBug("unimplemented: if, conditional jump");
	//set return symbol
	Entity::updateCurrentScope(sc);
	compilerBug("unimplemented: if, set return symbol");
}
