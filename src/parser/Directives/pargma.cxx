#include "../parser.hxx"

#include <ABI.h>

void parse::Directives::Pragma()
{
	ParserState.Token = ParserState.Line.nextToken();
	if (ParserState.Token.text == "push") {
		pragma::push();
	} else if (ParserState.Token.text == "pop") {
		pragma::pop();
	} else if (ParserState.Token.text == "warning") {
		ParserState.Token = ParserState.Line.nextToken();
		if (ParserState.Token.text == "enable") {
			ParserState.Token = ParserState.Line.nextToken();
			issues::changeGroupAction(ParserState.Token.text,issues::action::warning);
		} else if (ParserState.Token.text == "disable") {
			ParserState.Token = ParserState.Line.nextToken();
			issues::changeGroupAction(ParserState.Token.text,issues::action::ignore);
		} else if (ParserState.Token.text == "error") {
			ParserState.Token = ParserState.Line.nextToken();
			issues::changeGroupAction(ParserState.Token.text,issues::action::error);
		}
	} else if (ParserState.Token.text == "cpl") {
		ParserState.Token = ParserState.Line.nextToken();
		options::fcpl = atoi(ParserState.Token.text.c_str());
	} else if (ParserState.Token.text == "ABI") {
		ParserState.Token = ParserState.Line.nextToken();
		noSuchABI::error.push([](noSuchABI e) -> int {return 0;});
		ABI* abi = getABI(ParserState.Token.text);
		if(abi == nullptr)
		{
			noSuchABI::error.pop();
			noSuchABI("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),ParserState.Token.text);
		}
		noSuchABI::error.pop();
		defaultABI = abi;
	} else if(ParserState.Token.text == "once") {
		bool found = false;
		for(std::string& i : includedFiles)
		{
			if(i == ParserState.Line.file)
				parse::EndBlock();
		}
	}
}