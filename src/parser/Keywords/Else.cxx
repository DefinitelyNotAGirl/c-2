#include "../parser.hxx"

void parse::Keywords::Else()
{
	if(options::ddebug)std::cout << "else" << std::endl;
	scope* sc = new scope;
	sc->leadingSpace=currentScope->leadingSpace+tabLength;
	sc->name=currentScope->name+CPE2_SYMBOL_SCOPE_SEP"else"+std::to_string(currentScope->elseCounter++);
	sc->parent = currentScope;
	sc->isIndentBased = true;
	sc->t = scopeType::LOGICAL;
	sc->func = new function;
	*(sc->func) = *(currentScope->func);
	sc->func->code = new section;
	sc->reentrySymbol = currentScope->name+CPE2_SYMBOL_SCOPE_SEP"conditional"+std::to_string(sc->parent->conditionalCounter)+CPE2_SYMBOL_SCOPE_SEP"reentry";
	//set return symbol
	Entity::updateCurrentScope(sc);
	code->placeSymbol(SymbolType::CodeLocation,0,sc->name);
}
