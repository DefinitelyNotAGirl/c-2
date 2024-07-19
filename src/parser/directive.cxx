#include "parser.hxx"

void parse::Directive()
{
	if (ParserState.Token.text == "#EOL") {
		parse::EndLine();
	} else if (ParserState.Token.text == "#EOF") {
		parse::EndBlock();
	} else if (ParserState.Token.text == "#include") {
		parse::Directives::Include();
	} else if (ParserState.Token.text == "#pragma") {
		parse::Directives::Pragma();
	} else if (ParserState.Token.text == "#autodecl") {
		parse::Directives::Autodecl();
	}
}
