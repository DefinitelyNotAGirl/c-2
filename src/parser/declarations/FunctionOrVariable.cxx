#include "../parser.hxx"

void parse::Declaration::FunctionOrVariable()
{
	ParserState.DeclarationData.Type = getType(ParserState.Token.text);
	ParserState.NextToken();
	//+
	//+ check for array type
	//+
	{
		line arrayCountLine = ParserState.Line;
		arrayCountLine.tpos = 0;
		arrayCountLine.text = "";
		if(ParserState.Token.type == 32)
		{
			ParserState.NextToken();
			while(ParserState.Token.type != 33)
			{
				arrayCountLine.text += ParserState.Token.text;
				ParserState.NextToken();
			}
			token sizeToken = arrayCountLine.nextToken();
			ParserState.DeclarationData.ArraySize = resolve(sizeToken);
			if(ParserState.DeclarationData.ArraySize->storageArch != Architecture::storage_IntegerImmediate)
				nonImmediateArraySize("",originCoreHere,source(ParserState.File,ParserState.Line,sizeToken),ParserState.DeclarationData.Type);
			ParserState.DeclarationData.Type = getType(ParserState.DeclarationData.Type->name+"*");
			ParserState.NextToken();
		}
	}
	ParserState.DeclarationData.NameToken = ParserState.Token;
	ParserState.NextToken();
	if(ParserState.Token.type == 30)
		parse::Declaration::Function();
	else
		parse::Declaration::Variable();
}
