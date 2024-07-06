#include <parser.hxx>

void parse::Lines(std::vector<line>& Lines, std::string File)
{
	ParserState_T SavedState = ParserState;
	if(!File.empty())
		ParserState.File = File;
	ParserState.Lines = Lines;
	ParserState.LineIterator = 0;
	for(;
		ParserState.LineIterator < ParserState.Lines.size();
		ParserState.LineIterator++;
	){
		ParserState.Line = ParserState.Lines[ParserState.LineIterator];
		parse::Line();
	}
	ParserState = SavedState;
}
