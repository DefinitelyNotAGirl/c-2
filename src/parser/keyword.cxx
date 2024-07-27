#include "parser.hxx"

void parse::Keyword()
{
	if(ParserState.Token.text == "enum")parse::Declaration::Enum();
	else if(ParserState.Token.text == "litop")parse::Declaration::Litop();
	else if(ParserState.Token.text == "class")parse::Declaration::Class();
	else if(ParserState.Token.text == "namespace")parse::Declaration::Namespace();
	else if(ParserState.Token.text == "return")parse::Keywords::Return();
	else if(ParserState.Token.text == "while")parse::Keywords::While();
	else if(ParserState.Token.text == "for")parse::Keywords::For();
	else if(ParserState.Token.text == "if")parse::Keywords::If();
	else if(ParserState.Token.text == "else")parse::Keywords::Else();
	else if(ParserState.Token.text == "elseIf")parse::Keywords::ElseIf();
	else if(ParserState.Token.text == "switch")parse::Keywords::Switch();
	else if(ParserState.Token.text == "case")parse::Keywords::Case();
	else if(ParserState.Token.text == "break")parse::Keywords::Break();
	else if(ParserState.Token.text == "try")parse::Keywords::Try();
	else if(ParserState.Token.text == "catch")parse::Keywords::Catch();
	else if(ParserState.Token.text == "throw")parse::Keywords::Throw();
	else if(ParserState.Token.text == "async")parse::Keywords::Async();
	else if(ParserState.Token.text == "template")parse::Keywords::Template();
	else if(ParserState.Token.text == "c2resource")parse::Keywords::c2Resource();
	else issues::invalidUseOfKeywordInScope(ParserState.Token.text,originCoreHere,source(ParserState.File,ParserState.Line,ParserState.Token),currentScope);
}
