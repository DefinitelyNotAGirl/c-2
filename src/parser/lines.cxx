#include <parser.hxx>

void parse::Lines(std::vector<line>& Lines, std::string File)
{
	ParserState_T SavedState = ParserState;
	if(!File.empty())
		ParserState.File = File;
	ParserState.Lines = Lines;
	ParserState.LineIterator = 0;
	try {
		for(;
			ParserState.LineIterator < ParserState.Lines.size();
			ParserState.LineIterator++;
		){
			ParserState.Line = ParserState.Lines[ParserState.LineIterator];
			try {
				parse::Line();
			} catch(dummy_parser_endline*) {
			}
		}
	} catch(dummy_parser_endblock*) {
	}
	ParserState = SavedState;
}
