#include "parser.hxx"

void parse::Line()
{
	ParserState.Token = ParserState.Line.nextToken();
}
