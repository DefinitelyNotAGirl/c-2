#include "../parser.hxx"

#include <output.hxx>

uint64_t nextTypeID = 1;
void parse::Keywords::c2Resource()
{
	ParserState.Token = ParserState.Line.nextToken();
	if(ParserState.Token.text == "typeid")
	{
		ParserState.Token = ParserState.Line.nextToken();
		setGlobalValue(std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"typeid"+CPE2_SYMBOL_SCOPE_SEP+ParserState.Token.text,nextTypeID++);
	}
	else
		compilerBug("invalid resource type",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),"");
}
