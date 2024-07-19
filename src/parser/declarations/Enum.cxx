#include "../parser.hxx"

void parse::Declaration::Enum()
{
	ParserState.NextToken();
	switch (ParserState.Token.type) {
		case (1): {
			type* ntype		  = new type;
			mangler* mangling = defaultMangler;
			ntype->name		  = ParserState.Token.text;
			token nametoken = ParserState.Token;
			ntype->size		  = 8;
			ntype->__declared_file = currentFile;
			ntype->__declared_line = ParserState.Line.lineNum;
			if (ntype->name.back() == ':')
				ntype->name.pop_back();
			mangling->mangle(ntype);
			if (ParserState.Line.text.back() == ':') {
				// indent based body
				scope* sc  = new scope;
				sc->parent = currentScope;
				sc->name = currentScope->name + CPE2_SYMBOL_SCOPE_SEP +ntype->mangledName;
				sc->leadingSpace =
					ParserState.Line.leadingSpaces + tabLength;
				sc->isIndentBased = true;
				sc->t			  = scopeType::ENUM;
				sc->cl			  = ntype;
				Entity::updateCurrentScope(sc);
			} else {
			}
			types.push_back(ntype);
			break;
		}
		default:
			unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{9});
	}
}
