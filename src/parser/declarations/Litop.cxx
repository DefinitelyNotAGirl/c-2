#include "../parser.hxx"

void parse::Declaration::Litop()
{
	ParserState.NextToken();
	switch (ParserState.Token.type) {
		case (1): {
			litop* lop = new litop;
			lop->name  = ParserState.Token.text;
			ParserState.Token		   = ParserState.Line.nextToken();
			switch (ParserState.Token.type) {
				case (13):
					if (ParserState.Token.text == "add")
						lop->op = shortOP::ADD;
					else if (ParserState.Token.text == "sub")
						lop->op = shortOP::SUB;
					else if (ParserState.Token.text == "mul")
						lop->op = shortOP::MUL;
					else if (ParserState.Token.text == "div")
						lop->op = shortOP::DIV;
					else if (ParserState.Token.text == "cast")
						lop->op = shortOP::CAST;
					if (lop->op == shortOP::CAST) {
						ParserState.NextToken();
						switch (ParserState.Token.type) {
							case (9): {
								type* ct = getType(ParserState.Token.text);
								break;
							}
							default:
								delete lop;
								unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{9});
						}
					} else {
						ParserState.NextToken();
						variable* lv = resolve(ParserState.Token);
						if(lv->storageArch != Architecture::storage_IntegerImmediate)
							nonImmediateLitop(lop->name,originCoreHere,source(currentFile,ParserState.Line,ParserState.Token));
						lop->value = (uint64_t)lv->storage;
						litops.push_back(lop);
					}
					break;
				default:
					unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{13});
			}
			break;
		}
		default:
			unexpectedTokenType("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),{1});
	}
}
