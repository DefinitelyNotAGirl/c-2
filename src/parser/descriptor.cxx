#include "parser.hxx"

void parse::Descriptor()
{
	if(ParserState.Token.text == "@desc")
	{
		if(ParserState.currentd->desc.length() != 0)
			ParserState.currentd->desc.push_back('\x1A');
		ParserState.currentd->desc+=ParserState.Line.restText();
	}
	else if(ParserState.Token.text == "@return")
	{
		if(ParserState.currentd->ret.length() != 0)
			ParserState.currentd->ret.push_back('\x1A');
		ParserState.currentd->ret+=ParserState.Line.restText();
	}
	else if(ParserState.Token.text == "@param")
	{
		pdobj* param = new pdobj;
		param->name = ParserState.Line.nextToken().text;
		param->desc = ParserState.Line.restText();
		ParserState.currentd->params.push_back(param);
	}
	else if(ParserState.Token.text == "@tparam")
	{
		pdobj* tparam = new pdobj;
		tparam->name = ParserState.Line.nextToken().text;
		tparam->desc = ParserState.Line.restText();
		ParserState.currentd->params.push_back(tparam);
	}	
}
