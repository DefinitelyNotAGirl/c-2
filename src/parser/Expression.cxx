#include "parser.hxx"

void parse::Expression()
{
	//if(isConstExprAssignment)
	//	std::cout << "assigning constexpr" << std::endl;
	//std::cout << "resolving line: " << ParserState.Line.text << std::endl;
	variable* var = getVariable(ParserState.Token.text);
	token backupToken = ParserState.Token;
	line backupLine = ParserState.Line;
	ParserState.Token = ParserState.Line.nextToken();
	if(ParserState.Token.type == 32)
	{
		line indexLine;
		indexLine.lineNum = ParserState.Line.lineNum;
		indexLine.file = ParserState.Line.file;
		uint64_t l = 0;
		do {
			switch(ParserState.Token.type)
			{
				case(0):
					unexpectedBufferTermination("array index expression, logical line terminated",originCoreHere,source());
				case(32):
					l++;
					goto __default_aidx;
				case(33):
					l--;
				default:
					__default_aidx:;
					indexLine.text += ParserState.Token.text;
			}
			ParserState.Token = ParserState.Line.nextToken();
		} while(l > 0);
		indexLine.text = indexLine.text.substr(1,indexLine.text.length()-2);
		token indexToken = indexLine.nextToken();
		variable* index = resolve(indexToken);
		if(index->storageArch == Architecture::storage_IntegerImmediate)
		{
			uint64_t disp = (uint64_t)index->storage*(var->dataType->valueType->size);
			variable* element = new variable;
			element->name = "____cpe2____arrayElement";
			element->dataType = var->dataType->valueType;
			element->storageArch = currentArchitecture;
			if(currentArchitecture == Architecture::AMD64)
			{
				//TODO: check if var is stored in register, if not, move to rbx
				amd64::VariableStorage* storage = new amd64::VariableStorage;
				element->storage = storage;
				storage->mode = amd64::StorageMode::IndirectRegister;
				storage->reg = ((amd64::VariableStorage*)var->storage)->reg;
				storage->displacement = ImmediateValue(disp);
				var = element;
			}
		}
		else compilerBug("unimplemented: non-immediate array index");
		
	}
	if(ParserState.Token.text.back() == '=')
	{
		token ot = ParserState.Token;
		ParserState.Token = ParserState.Line.nextToken();
		variable* result = resolve(ParserState.Token);
		std::vector<variable*> args;
		args.push_back(var);
		args.push_back(result);
		std::string fname = "operator"+ot.text;
		function* func = getFunction(fname, args);
		if (func->isDeprecated)
			warn(getWarning("deprecated"), &ParserState.Line,
				 "call to deprecated function \"" +
					 func->name + "\"");
		call(func, args);
	}
	else
	{
		ParserState.Line = backupLine;
		ParserState.Token = backupToken;
		resolve(ParserState.Token);
	}
}
