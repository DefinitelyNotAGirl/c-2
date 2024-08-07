#include "../parser.hxx"
#include <event.hxx>

void parse::Declaration::Class()
{
	ParserState.NextToken();
	ParserState.DeclarationData.NameToken = ParserState.Token;
	ParserState.NextToken();
	std::vector<token> inherit;
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
					inherit.push_back(ParserState.Token);
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
		Type = Entity::startTypeDefinition(ParserState.Attributes,ParserState.DeclarationData.NameToken.text,inherit,ParserState.Token.type == 40);
	} else if (ParserState.Token.type == 41) {
		Type = Entity::declareType(ParserState.Attributes,ParserState.DeclarationData.NameToken.text,inherit);
	} else {
		unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{40,41});
	}
	Event::Data::TokenIdentified EventData;
	ParserState.DeclarationData.NameToken.type = 9;
	EventData.Token = &ParserState.DeclarationData.NameToken;
	EventData.obj = (void*)Type;
	Event::TokenIdentified.fire(&EventData);
	//if(templateMode == 0)
	//{
	//	declareDwarfType(ntype);
	//	types.push_back(ntype);
	//	if(is_vstc_send && !isTemplateInstance.top() && nametoken.lineNum != 0)std::cout << "0001\x0c" << nametoken.lineNum <<'\x0c'<< nametoken.tcol <<'\x0c'<< nametoken.text.length() <<'\x0c'<<nametoken.text<<'\x0c'<<ntype->__declared_file<<'\x0c'<<std::to_string(ntype->__declared_line)<<'\x0c'<<ntype->desc<<'\n';
	//}
}
