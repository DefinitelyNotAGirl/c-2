#include "parser.hxx"

extern std::string __reqFileVSTC;
void parse::Lines(std::vector<line>& Lines, std::string File)
{
	ParserState_T SavedState = ParserState;
	//if(!File.empty()) {
	ParserState.File = File;
	//}
	ParserState.Lines = Lines;
	ParserState.LineIterator = 0;
	try {
		for(;
			ParserState.LineIterator < ParserState.Lines.size();
			ParserState.LineIterator++
		){
			ParserState.Line = ParserState.Lines[ParserState.LineIterator];
			try {
				parse::Line();
			} catch(issue e) {
			} catch(dummy_parser_endline*) {
			}
			ParserState.Attributes.clear();
		}
	} catch(dummy_parser_endblock*) {
	}
	ParserState = SavedState;
}
