#include "../parser.hxx"

using namespace issues;

void parse::Keywords::For()
{
	ParserState.Token = ParserState.Line.nextToken();
	//construct new scope
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"forloop"+std::to_string(currentScope->forCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = true;
	sc->t = scopeType::LOGICAL;
	sc->func = new function;
	//sc->parent->conditionalCounter++;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	//currentScope->reentrySymbol = sc->parent->name+CPE2_SYMBOL_SCOPE_SEP"conditional"+std::to_string(sc->parent->conditionalCounter)+CPE2_SYMBOL_SCOPE_SEP"reentry";
	//sc->extraCodeBlocks.push_back(&sc->func->code);
	Entity::updateCurrentScope(sc);
	compilerBug("unimplemented: for, place symbol currentScope->name");
	//parse begin line
	line beginLine = ParserState.Line;
	beginLine.text = ParserState.Line.restText();
	beginLine.tpos = 0;
	beginLine.leadingSpaces+=tabLength;
	std::vector<line> beginLines = {beginLine};
	parse::Lines(beginLines);
	//parse condition line
	sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"body";
	sc->parent = currentScope;
	sc->isIndentBased = true;
	SETBIT_00(sc->miscData);
	sc->t = scopeType::CONDITIONAL_BLOCK;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	sc->reentrySymbol = sc->parent->name+CPE2_SYMBOL_SCOPE_SEP"reentry";
	compilerBug("unimplemented: for, set reentry symbol");
	line conditionLine = ParserState.Lines[++ParserState.LineIterator];
	token cond = conditionLine.nextToken();
	variable* condition = resolve(cond);
	compilerBug("unimplemented: for, conditional jump");
	//parse end line
	Entity::updateCurrentScope(sc);
	line endLine = ParserState.Lines[++ParserState.LineIterator];
	{
		uint64_t bi = endLine.text.size()-1;
		while(endLine.text[bi] != ':' && endLine.text[bi] != '{')
			bi--;
		while(endLine.text[bi] != ')')
			bi--;
		endLine.text = endLine.text.substr(0,bi);
	}
	endLine.leadingSpaces = currentScope->leadingSpace;
	std::vector<line> endLines = {endLine};
	parse::Lines(endLines);
	//prepare for body
	section* endcode = new section;
	endcode->push(currentScope->func->code);
	currentScope->func->code = new section;
	currentScope->extraCodeBlocks.push_back(currentScope->func->code);
	currentScope->extraCodeBlocks.push_back(endcode);
	compilerBug("unimplemented: for, set body entry symbol");
}
