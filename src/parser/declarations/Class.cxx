#include "../parser.hxx"

void parse::Declaration::Class()
{
	ParserState.NextToken();
	ParserState.DeclarationData.NameToken = ParserState.Token;
	ParserState.NextToken();
	std::vector<std::string> inherit;
	token f = ParserState.Token;
	switch (ParserState.Token.type) {
		case (40):
			goto collectInheritance;
		case (8):
			if (ParserState.Token.text == "extends") {
			collectInheritance:;
				// collect types to inherit from
				int64_t startOffset = 0;
				ParserState.NextToken();
				if(ParserState.Token.type == 0)
				{
					ParserState.Token = f;
					goto noMoreInheritance;
				}
				while (ParserState.Token.type == 9 ||
					   ParserState.Token.type == 12) {
					inherit.push_back(ParserState.Token.text);
					ParserState.NextToken();
					switch(ParserState.Token.type)
					{
						case(40):
						case(41):
							goto noMoreInheritance;
						case(42):
							break;
						default:
							unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{40,41,42});
					}
					ParserState.NextToken();
				}
				noMoreInheritance:;
			}	
			break;
	}
	type* Type = nullptr;
	if (ParserState.Token.type == 40 || ParserState.Token.type == 36) {
		scope* sc		  = new scope;
		sc->parent		  = currentScope;
		sc->name = currentScope->name + CPE2_SYMBOL_SCOPE_SEP +Type->mangledName;
		sc->leadingSpace = ParserState.Line.leadingSpaces + tabLength;
		sc->isIndentBased = ParserState.Token.type == 40;
		sc->func = nullptr;
		sc->t = scopeType::CLASS;
		sc->cl = Type;
		//if(templateMode == 3)
		//{
		//	sc->templateMode = true;
		//	__typeTemplate = new typeTemplate;
		//	__typeTemplate->tArgs = templateArgs;
		//	__typeTemplate->name = ntype->name;
		//	__typeTemplate->sc = currentScope;
		//	templateMode = 1;
		//}
		updateCurrentScope(sc);
	} else if (ParserState.Token.type == 41) {
	} else {
		unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{40,41});
	}
	//if(templateMode == 0)
	//{
	//	declareDwarfType(ntype);
	//	types.push_back(ntype);
	//	if(is_vstc_send && !isTemplateInstance.top() && nametoken.lineNum != 0)std::cout << "0001\x0c" << nametoken.lineNum <<'\x0c'<< nametoken.tcol <<'\x0c'<< nametoken.text.length() <<'\x0c'<<nametoken.text<<'\x0c'<<ntype->__declared_file<<'\x0c'<<std::to_string(ntype->__declared_line)<<'\x0c'<<ntype->desc<<'\n';
	//}
}
