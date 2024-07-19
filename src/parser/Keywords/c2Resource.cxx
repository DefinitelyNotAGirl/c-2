#include "../parser.hxx"

#include <output.hxx>

void parse::Keywords::c2Resource()
{
	ParserState.Token = ParserState.Line.nextToken();
	if(ParserState.Token.text == "exception")
	{
		emitExceptionSymbols = true;
		ParserState.Token = ParserState.Line.nextToken();
		std::string sym_routine = std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"exceptions"+CPE2_SYMBOL_SCOPE_SEP+ParserState.Token.text+CPE2_SYMBOL_SCOPE_SEP+"routine";
		std::string sym_datadst = std::string("____cpe2")+CPE2_SYMBOL_SCOPE_SEP+"exceptions"+CPE2_SYMBOL_SCOPE_SEP+ParserState.Token.text+CPE2_SYMBOL_SCOPE_SEP+"datadst";
		setGlobalValue(sym_routine,exceptionoffset+0);
		setGlobalValue(sym_datadst,exceptionoffset+8);
		exceptionoffset+=16;
	}
	else
		compilerBug("invalid resource type",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),"");
}
