#include "../parser.hxx"
#include <event.hxx>

void parse::Declaration::Function()
{
	std::vector<variable*> arguments;
	bool hasMoreParams = true;
	ParserState.NextToken();
	while (true) {
		variable* arg = new variable;
		arg->isParameter = true;
		arg->__declared_file = ParserState.File;
		arg->__declared_line = ParserState.Line.lineNum;
		switch(ParserState.Token.type)
		{
			case(42):
				ParserState.NextToken();
				break;
			case(31):
				goto FUNCTIONNOARGS;
		}
		switch (ParserState.Token.type) {
			case (9):
				arg->dataType = getType(ParserState.Token.text);
				break;
			case(31):
				arguments.push_back(arg);
				goto FUNCTIONNOARGS;
				break;
			default:
				unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{31,9});
		}
		ParserState.NextToken();
		switch (ParserState.Token.type) {
			case(1): {
				arg->name = ParserState.Token.text;
				Event::Data::TokenIdentified EventData;
				ParserState.Token.type = 60;
				EventData.Token = &ParserState.Token;
				EventData.obj = arg;
				Event::TokenIdentified.fire(&EventData);
				ParserState.NextToken();
				break;
			} case(31):
				arguments.push_back(arg);
				goto FUNCTIONNOARGS;
				break;
			case(42):
				arg->name = getNewName();
				ParserState.NextToken();
				break;
			default:
				unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{1});
		}
		arguments.push_back(arg);
	}
	FUNCTIONNOARGS:;
	ParserState.NextToken();
	function* func;
	if (ParserState.Token.type == 40 || ParserState.Token.type == 36) {
		func = Entity::startFunctionDefinition(
			ParserState.Attributes,
			ParserState.DeclarationData.NameToken.text,
			arguments,
			ParserState.DeclarationData.Type,
			ParserState.Token.type == 40
		);
	} else if (ParserState.Token.type == 41) {
		func = Entity::declareFunction(
			ParserState.Attributes,
			ParserState.DeclarationData.NameToken.text,
			arguments,
			ParserState.DeclarationData.Type
		);
	} else unexpectedTokenType("",originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),{40,36,41});
	Event::Data::TokenIdentified EventData;
	ParserState.DeclarationData.NameToken.type = 11;
	EventData.Token = &ParserState.DeclarationData.NameToken;
	EventData.obj = func;
	Event::TokenIdentified.fire(&EventData);
}
