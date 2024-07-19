#include <EntityManagement.hxx>
#include <output.hxx>
#include <Parser.hxx>

using namespace issues;

/**
 * @brief returns true if more bodies may need to be closed
 * 
 * @param ts 
 * @return true 
 * @return false 
 */
bool Entity::close(scope* ts)
{
	if (options::ddebug) std::cout << "body ended: " << ts->name << std::endl;
	bool goToParentScope = true;
	if ( //. this accounts for all the things that can be inside functions, grouped in order to handle templates
		ts->t == scopeType::FUNCTION
		|| ts->t == scopeType::CONDITIONAL_BLOCK
		|| ts->t == scopeType::LOGICAL
		|| ts->t == scopeType::DUMMY
		|| ts->t == scopeType::TRY_CATCH
		|| ts->t == scopeType::TRY
		|| ts->t == scopeType::CATCH
	) {
		//std::cout << "ended body: " << ts->func->symbol << std::endl;
		//std::cout << "scope: " << ts->name << std::endl;
		//std::cout << "blocks: " << ts->extraCodeBlocks.size() << std::endl;
		//if(ts->templateMode)
		//{
		//	std::cout << "template complete" << std::endl;
		//	if(templateMode == 1)
		//	{
		//		printTypeTemplate(__typeTemplate);
		//		typeTemplates.push_back(__typeTemplate);
		//	}
		//	templateArgs.clear();
		//	__typeTemplate = nullptr;
		//	__functionTemplate = nullptr;
		//	templateMode = 0;
		//}
		if(ts->t == scopeType::FUNCTION)
		{
			section imcode;
			if(!ts->func->isLocal)
				imcode.placeSymbol(SymbolType::GlobalFunction,0,ts->func->symbol);
			else
				imcode.placeSymbol(SymbolType::LocalFunction,0,ts->func->symbol);
			//,
			//, generic prologue
			//,
			{
				if(ts->func->stack->size() > 0)
				{
					imcode.push({amd64::opcode::enter::rBP__imm16__imm8});
					imcode.push(amd64::imm16(ts->func->stack->size()));
					imcode.push({(byte)0});
				}
			}
			ts->func->abi->genProlouge(&imcode, ts);
			imcode.push(ts->func->code);
			ts->func->abi->genEpilouge(&imcode, ts);
			//,
			//, generic epilogue
			//,
			{
				std::string sym = ts->func->symbol+".epilogue";
				imcode.placeSymbol(SymbolType::CodeLocation,0,sym);
				if(ts->func->stack->size() > 0)
					imcode.push(amd64::opcode::leave::rBP);
				imcode.push(amd64::opcode::ret_near::_);
			}
			for(section* block : ts->extraCodeBlocks)
				imcode.push(block);
			imcode.symbols[0].size = imcode.size();
			text.push(&imcode);
			imcode.data = nullptr;
			unimplementedDebugInfo("function");
		}
		else if(ts->t == scopeType::CONDITIONAL_BLOCK)
		{
			if(EXPR_GETBIT_00(ts->miscData))
			{
				uint64_t IV = 0;
				ts->extraCodeBlocks[0]->push(ts->extraCodeBlocks[1]);
				for(section* block : ts->extraCodeBlocks)
				{
					if(IV != 1)
					{
						ts->parent->extraCodeBlocks.push_back(block);
					}
					IV++;
				}
				runtime::RelativeControlTransfer(ts->reentrySymbol);
			}
			else
			{
				for(section* block : ts->extraCodeBlocks)
					ts->parent->extraCodeBlocks.push_back(block);
				runtime::RelativeControlTransfer(ts->reentrySymbol);
			}
			//else
			//    std::cout << ts->fstore->stackSize << " <= " << ts->parent->fstore->stackSize << std::endl;
			//if(ts->fstore->stackOffset > ts->parent->fstore->stackOffset)
			//    ts->parent->fstore->stackOffset = ts->fstore->stackOffset;
		}
		else if(ts->t == scopeType::LOGICAL)
		{
			ts->parent->func->code->push(ts->func->code);
			for(section* block : ts->extraCodeBlocks)
				ts->parent->extraCodeBlocks.push_back(block);
		}
		else if(ts->t == scopeType::DUMMY)
		{
			ts->parent->func->code->push(ts->func->code);
		}
		else if(ts->t == scopeType::TRY)
		{
			ts->parent->func->code = ts->func->code;
			for(uint64_t I = 0;I<ts->extraCodeBlocks.size();I++)
				ts->parent->extraCodeBlocks.push_back(ts->extraCodeBlocks[I]);
		}
		else if(ts->t == scopeType::CATCH)
		{
			runtime::RelativeControlTransfer(ts->parent->reentrySymbol);
			//ts->parent->extraCodeBlocks[0]->push(ts->extraCodeBlocks[0]);
			//ts->parent->extraCodeBlocks[1]->push(ts->extraCodeBlocks[1]);
			//ts->parent->extraCodeBlocks.push_back(ts->func->code);
			for(uint64_t I = 2;I<ts->extraCodeBlocks.size();I++)
				ts->parent->extraCodeBlocks.push_back(ts->extraCodeBlocks[I]);
		}
		else if(ts->t == scopeType::TRY_CATCH)
		{
			if(ParserState.Token.type == 8 && ParserState.Token.text == "catch")
			{
				goToParentScope = false;
			}
			else
			{
				ts->parent->func->code->push(ts->extraCodeBlocks[0]);//install handlers
				ts->parent->func->code->push(ts->func->code);//run try
				ts->parent->func->code->push(ts->extraCodeBlocks[1]);//reset handlers
				for(uint64_t I = 2;I<ts->extraCodeBlocks.size();I++)//pass the extra blocks further up
					ts->parent->extraCodeBlocks.push_back(ts->extraCodeBlocks[I]);
			}
		}
	} else if (ts->t == scopeType::CLASS) {
		//targTypes.clear();
		//std::cout << "ended class body: " << ts->cl->mangledName << std::endl;
		ts->cl->incomplete = false;
		//if(ts->templateMode)
		//{
		//	//std::cout << "template complete" << std::endl;
		//	if(templateMode == 1)
		//	{
		//		//printTypeTemplate(__typeTemplate);
		//		typeTemplates.push_back(__typeTemplate);
		//	}
		//	templateArgs.clear();
		//	__typeTemplate = nullptr;
		//	__functionTemplate = nullptr;
		//	templateMode = 0;
		//}
	} else if (ts->t == scopeType::NAMESPACE) {
		//std::cout << "ended namespace body: " << ts->name << std::endl;
	}
	if(goToParentScope)
	{
		if(options::ddebug)
		{
			std::cout << "ended scope: " << ts->name << std::endl;
		}
		ts = ts->parent;
		return false;
	}
	return true;
}
