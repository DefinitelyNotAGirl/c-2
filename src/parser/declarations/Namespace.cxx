#include "../parser.hxx"

void parse::Declaration::Namespace()
{
	std::vector<std::string> nsp;
	ParserState.NextToken();
	bool wasColon = false;
	while(true)
	{
		if(ParserState.Token.type == 40)
			if(wasColon)
				wasColon = false;
			else
			{
				wasColon = true;
				break;
			}
		else if(ParserState.Token.type == 1)
			nsp.push_back(ParserState.Token.text);
		ParserState.NextToken();
	}
	std::string symb = currentScope->name;
	scope* sco		 = nullptr;
	for (std::string ns : nsp) {
		scope* sc = new scope;
		sc->t	  = scopeType::NAMESPACE;
		if (!symb.empty()) symb += CPE2_SYMBOL_SCOPE_SEP;
		symb += ns;
		sc->name   = symb;
		sc->parent = currentScope;
		if (sco) sc->lparent = sco;
		else sc->lparent = currentScope;
		sco = sc;
		if(options::ddebug)
			std::cout << "declared namespace: " << symb<< std::endl;
	}
	if(wasColon) 
	{
		sco->leadingSpace = ParserState.Line.leadingSpaces + tabLength;
		Entity::updateCurrentScope(sco);
	}
	//sco->attribs = ParserState.Attributes;
}
