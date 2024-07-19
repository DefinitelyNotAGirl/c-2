#include "../parser.hxx"

void parse::Declaration::Variable()
{
	variable* var = Entity::defineVariable(ParserState.Attributes,ParserState.DeclarationData.NameToken.text,ParserState.DeclarationData.Type);
	ParserState.NextToken();
	switch (ParserState.Token.type) {
		case (3):
		case( 6):
		case( 7):
		case(10):
		case(11):
		case( 1):
		case( 9):
		{
			variable* result = resolve(ParserState.Token);
			std::vector<variable*> args;
			args.push_back(var);
			args.push_back(result);
			std::string cppcock = "operator=";
			function* func = getFunction(voidType,cppcock,args);
			if(var->isConstexpr)
			{
				if(!func->isPrimitive)
					nonPrimitiveOperationOnConstexpr("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token));
				if(result->storageArch != Architecture::storage_IntegerImmediate)
					dynamicAssignmentToConstexpr("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token));
				if(result->storageArch == Architecture::storage_IntegerImmediate)
					var->storage = result->storage;
			}
			else
			{
				if (func->isDeprecated)
				{
					//TODO: deprecation warning
				}
				call(func, args);
			}
			break;
		}
		default:
		{
			if(var->dataType->ctor != nullptr)
			{
				std::vector<variable*> args = {var};
				call(var->dataType->ctor,args);
			}
			break;
		}
	}
}
