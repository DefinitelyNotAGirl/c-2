/*
 * Created Date: Wednesday May 22nd 2024
 * Author: Lilith
 * -----
 * Last Modified: Sun Aug 04 2024
 * Modified By: Lilith
 * -----
 * Copyright (c) 2023-2024 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <colors.h>
#include <stacktrace.hxx>
#include <stack>
#include <string>

class function;
class type;
class token;
#include <class_line.h>
#include <list>
class scope;

namespace issues {
	#define originCoreHere issues::origin("core",__FILE__,__LINE__)

	extern uint64_t ErrorCount;

	enum class action {
		error,
		warning,
		info,
		ignore
	};

	template<typename issueType>
	void invoke(issueType e)
	{
		switch(e.Action)
		{
			case(action::error):
			{
				if(issueType::error.top()(e) == 1)
					throw e;
				break;
			}
			case(action::warning):
			{
				issueType::warn.top()(e);
				break;
			}
			case(action::info):
			{
				issueType::info.top()(e);
				break;
			}
		}
	}

	/**
	 * @brief stores data on where an issue originated
	 */
	class origin {
	public:
		std::string module;
		std::string file;
		uint64_t line;
		origin(std::string module, std::string file, uint64_t line)
			:module(module),file(file),line(line){}
	};

	/**
	 * @brief stores data a location in the source code
	 */
	class source {
	public:
		bool present = true;
		std::string sourceFile;
		line sourceLine;
		token sourceToken;
		source(){present = false;}
		source(std::string sourceFile, line sourceLine, token sourceToken)
			:sourceFile(sourceFile),sourceLine(sourceLine),sourceToken(sourceToken){}

		void print()
		{
			if(this->present == false)return;
			std::cout << this->sourceFile << ": \n";
			std::string lnstr = std::to_string(this->sourceLine.lineNum);
			std::cerr << "  " << lnstr << " | " << this->sourceLine.text << "\n";
			for(uint64_t i = 0;i<(this->sourceToken.tcol+lnstr.length()+5);i++)
				std::cerr << ' ';
			std::cerr << COLOR_RED << "^" << COLOR_RESET << std::endl;
		}
	};

	class issue {
	public:
		static const uint64_t stackTraceMaxLength = 50;
		void** stackTraceData;
		size_t stackTraceLength;
		std::string msg;
		std::list<origin> trace;//using list over vector for push_front
		source src;

		issue()
		{
			this->stackTraceData = (void**)calloc(issue::stackTraceMaxLength,8);
			this->stackTraceLength = backtrace(this->stackTraceData, issue::stackTraceMaxLength);
		}

		void printStackTrace()
		{
			std::cerr << "stack trace: ";
			for(uint64_t I = 3;I<this->stackTraceLength;I++)
			{
				std::string info = get_source_info(this->stackTraceData[I]);
				if(!info.empty())std::cerr << "\n" << info;
			}
			std::cerr << std::endl;
		}

		void printTrace()
		{
			if(this->trace.size() > 0)
			{
				std::cerr << "issue trace: " << "\n";
				for(origin& orig : this->trace)
					std::cerr << "    " << orig.module << " " << orig.file << ":" << orig.line << "\n";
			}
		}
	};

//+####################################################################################################################
//+####################################################################################################################
//+ ███████  █████  ████████  █████  ██          ███████ ██████  ██████   ██████  ██████  ███████
//+ ██      ██   ██    ██    ██   ██ ██          ██      ██   ██ ██   ██ ██    ██ ██   ██ ██
//+ █████   ███████    ██    ███████ ██          █████   ██████  ██████  ██    ██ ██████  ███████
//+ ██      ██   ██    ██    ██   ██ ██          ██      ██   ██ ██   ██ ██    ██ ██   ██      ██
//+ ██      ██   ██    ██    ██   ██ ███████     ███████ ██   ██ ██   ██  ██████  ██   ██ ███████
//+####################################################################################################################
//+####################################################################################################################
/*
 + Fatal errors are errors that cannot be downgraded to warnings
*/
	class fatal : public issue {
	public:
		const static action Action = action::error;
	};

	#define ISSUES_CTOR_ARGS std::string msg, origin orig, source src
	#define ISSUES_CTOR_INIT this->msg = msg;this->trace.push_front(orig);this->src = src

	class compilerBug : public fatal {
	public:
		std::string github; //link to issue, empty string if no issue exists
		static std::stack<void(*)(compilerBug e)> info;
		static std::stack<void(*)(compilerBug e)> warn;
		static std::stack<int(*)(compilerBug e)> error;
		compilerBug(ISSUES_CTOR_ARGS, std::string github)
			:github(github){ISSUES_CTOR_INIT;invoke(*this);}
		compilerBug(std::string msg)
			:github(""){this->msg = msg;this->src = source();invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(compilerBug e)> compilerBug::info;
		std::stack<void(*)(compilerBug e)> compilerBug::warn;
		std::stack<int(*)(compilerBug e)> compilerBug::error;
	#endif

	class unexpectedTokenType : public fatal {
	public:
		std::list<uint64_t> expectedTokenTypes;
		static std::stack<void(*)(unexpectedTokenType e)> info;
		static std::stack<void(*)(unexpectedTokenType e)> warn;
		static std::stack<int(*)(unexpectedTokenType e)> error;
		unexpectedTokenType(ISSUES_CTOR_ARGS, std::list<uint64_t> expectedTokenTypes)
			:expectedTokenTypes(expectedTokenTypes){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(unexpectedTokenType e)> unexpectedTokenType::info;
		std::stack<void(*)(unexpectedTokenType e)> unexpectedTokenType::warn;
		std::stack<int(*)(unexpectedTokenType e)> unexpectedTokenType::error;
	#endif

	class noSuchFile : public fatal {
	public:
		std::string file;//examples
						 //		"blub.h2"
						 //		<blub.h2>
		std::list<std::string> checkedPaths;
		static std::stack<void(*)(noSuchFile e)> info;
		static std::stack<void(*)(noSuchFile e)> warn;
		static std::stack<int(*)(noSuchFile e)> error;
		noSuchFile(ISSUES_CTOR_ARGS, std::string file, std::list<std::string> checkedPaths)
			:file(file),checkedPaths(checkedPaths){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchFile e)> noSuchFile::info;
		std::stack<void(*)(noSuchFile e)> noSuchFile::warn;
		std::stack<int(*)(noSuchFile e)> noSuchFile::error;
	#endif

	class noSuchType : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchType e)> info;
		static std::stack<void(*)(noSuchType e)> warn;
		static std::stack<int(*)(noSuchType e)> error;
		noSuchType(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchType e)> noSuchType::info;
		std::stack<void(*)(noSuchType e)> noSuchType::warn;
		std::stack<int(*)(noSuchType e)> noSuchType::error;
	#endif

	class noSuchIdentifier : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchIdentifier e)> info;
		static std::stack<void(*)(noSuchIdentifier e)> warn;
		static std::stack<int(*)(noSuchIdentifier e)> error;
		noSuchIdentifier(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchIdentifier e)> noSuchIdentifier::info;
		std::stack<void(*)(noSuchIdentifier e)> noSuchIdentifier::warn;
		std::stack<int(*)(noSuchIdentifier e)> noSuchIdentifier::error;
	#endif

	class invalidUseOfKeywordInScope : public fatal {
	public:
		scope* Scope;
		static std::stack<void(*)(invalidUseOfKeywordInScope e)> info;
		static std::stack<void(*)(invalidUseOfKeywordInScope e)> warn;
		static std::stack<int(*)(invalidUseOfKeywordInScope e)> error;
		invalidUseOfKeywordInScope(ISSUES_CTOR_ARGS, scope* Scope)
			:Scope(Scope){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(invalidUseOfKeywordInScope e)> invalidUseOfKeywordInScope::info;
		std::stack<void(*)(invalidUseOfKeywordInScope e)> invalidUseOfKeywordInScope::warn;
		std::stack<int(*)(invalidUseOfKeywordInScope e)> invalidUseOfKeywordInScope::error;
	#endif

	class noSuchABI : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchABI e)> info;
		static std::stack<void(*)(noSuchABI e)> warn;
		static std::stack<int(*)(noSuchABI e)> error;
		noSuchABI(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchABI e)> noSuchABI::info;
		std::stack<void(*)(noSuchABI e)> noSuchABI::warn;
		std::stack<int(*)(noSuchABI e)> noSuchABI::error;
	#endif

	class nonImmediateArraySize : public fatal {
	public:
		type* valueType;
		static std::stack<void(*)(nonImmediateArraySize e)> info;
		static std::stack<void(*)(nonImmediateArraySize e)> warn;
		static std::stack<int(*)(nonImmediateArraySize e)> error;
		nonImmediateArraySize(ISSUES_CTOR_ARGS, type* valueType)
			:valueType(valueType){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(nonImmediateArraySize e)> nonImmediateArraySize::info;
		std::stack<void(*)(nonImmediateArraySize e)> nonImmediateArraySize::warn;
		std::stack<int(*)(nonImmediateArraySize e)> nonImmediateArraySize::error;
	#endif

	class nonImmediateIntegerTemplateArgument : public fatal {
	public:
		static std::stack<void(*)(nonImmediateIntegerTemplateArgument e)> info;
		static std::stack<void(*)(nonImmediateIntegerTemplateArgument e)> warn;
		static std::stack<int(*)(nonImmediateIntegerTemplateArgument e)> error;
		nonImmediateIntegerTemplateArgument(ISSUES_CTOR_ARGS)
			{ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(nonImmediateIntegerTemplateArgument e)> nonImmediateIntegerTemplateArgument::info;
		std::stack<void(*)(nonImmediateIntegerTemplateArgument e)> nonImmediateIntegerTemplateArgument::warn;
		std::stack<int(*)(nonImmediateIntegerTemplateArgument e)> nonImmediateIntegerTemplateArgument::error;
	#endif

	class noSuchLitop : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchLitop e)> info;
		static std::stack<void(*)(noSuchLitop e)> warn;
		static std::stack<int(*)(noSuchLitop e)> error;
		noSuchLitop(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchLitop e)> noSuchLitop::info;
		std::stack<void(*)(noSuchLitop e)> noSuchLitop::warn;
		std::stack<int(*)(noSuchLitop e)> noSuchLitop::error;
	#endif

	class invalidAttribute : public fatal {
	public:
		std::string entityName;
		std::string attribute;
		std::list<std::string> validAttributes;
		static std::stack<void(*)(invalidAttribute e)> info;
		static std::stack<void(*)(invalidAttribute e)> warn;
		static std::stack<int(*)(invalidAttribute e)> error;
		invalidAttribute(ISSUES_CTOR_ARGS, std::string entityName,std::string attribute,std::list<std::string> validAttributes)
			:entityName(entityName),attribute(attribute),validAttributes(validAttributes){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(invalidAttribute e)> invalidAttribute::info;
		std::stack<void(*)(invalidAttribute e)> invalidAttribute::warn;
		std::stack<int(*)(invalidAttribute e)> invalidAttribute::error;
	#endif

	class noSuchFunction : public fatal {
	public:
		function* neededFunction;
		std::vector<function*> candidates;
		static std::stack<void(*)(noSuchFunction e)> info;
		static std::stack<void(*)(noSuchFunction e)> warn;
		static std::stack<int(*)(noSuchFunction e)> error;
		noSuchFunction(ISSUES_CTOR_ARGS,function* neededFunction, std::vector<function*> candidates)
			:neededFunction(neededFunction),candidates(candidates){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchFunction e)> noSuchFunction::info;
		std::stack<void(*)(noSuchFunction e)> noSuchFunction::warn;
		std::stack<int(*)(noSuchFunction e)> noSuchFunction::error;
	#endif

	class noSuchVariable : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchVariable e)> info;
		static std::stack<void(*)(noSuchVariable e)> warn;
		static std::stack<int(*)(noSuchVariable e)> error;
		noSuchVariable(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchVariable e)> noSuchVariable::info;
		std::stack<void(*)(noSuchVariable e)> noSuchVariable::warn;
		std::stack<int(*)(noSuchVariable e)> noSuchVariable::error;
	#endif

	class noSuchMangler : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchMangler e)> info;
		static std::stack<void(*)(noSuchMangler e)> warn;
		static std::stack<int(*)(noSuchMangler e)> error;
		noSuchMangler(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchMangler e)> noSuchMangler::info;
		std::stack<void(*)(noSuchMangler e)> noSuchMangler::warn;
		std::stack<int(*)(noSuchMangler e)> noSuchMangler::error;
	#endif

	class noSuchSystem : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchSystem e)> info;
		static std::stack<void(*)(noSuchSystem e)> warn;
		static std::stack<int(*)(noSuchSystem e)> error;
		noSuchSystem(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchSystem e)> noSuchSystem::info;
		std::stack<void(*)(noSuchSystem e)> noSuchSystem::warn;
		std::stack<int(*)(noSuchSystem e)> noSuchSystem::error;
	#endif

	class noSuchArchitecture : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchArchitecture e)> info;
		static std::stack<void(*)(noSuchArchitecture e)> warn;
		static std::stack<int(*)(noSuchArchitecture e)> error;
		noSuchArchitecture(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchArchitecture e)> noSuchArchitecture::info;
		std::stack<void(*)(noSuchArchitecture e)> noSuchArchitecture::warn;
		std::stack<int(*)(noSuchArchitecture e)> noSuchArchitecture::error;
	#endif

	class noSuchNumberSystem : public fatal {
	public:
		std::string name;
		static std::stack<void(*)(noSuchNumberSystem e)> info;
		static std::stack<void(*)(noSuchNumberSystem e)> warn;
		static std::stack<int(*)(noSuchNumberSystem e)> error;
		noSuchNumberSystem(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(noSuchNumberSystem e)> noSuchNumberSystem::info;
		std::stack<void(*)(noSuchNumberSystem e)> noSuchNumberSystem::warn;
		std::stack<int(*)(noSuchNumberSystem e)> noSuchNumberSystem::error;
	#endif

	class invalidType : public fatal {
	public:
		std::list<type*> validTypes;
		type* receivedType;
		static std::stack<void(*)(invalidType e)> info;
		static std::stack<void(*)(invalidType e)> warn;
		static std::stack<int(*)(invalidType e)> error;
		invalidType(ISSUES_CTOR_ARGS,std::list<type*> validTypes,type* receivedType)
			:validTypes(validTypes),receivedType(receivedType){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(invalidType e)> invalidType::info;
		std::stack<void(*)(invalidType e)> invalidType::warn;
		std::stack<int(*)(invalidType e)> invalidType::error;
	#endif

	class unexpectedBufferTermination : public fatal {
	public:
		static std::stack<void(*)(unexpectedBufferTermination e)> info;
		static std::stack<void(*)(unexpectedBufferTermination e)> warn;
		static std::stack<int(*)(unexpectedBufferTermination e)> error;
		unexpectedBufferTermination(ISSUES_CTOR_ARGS)
			{ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(unexpectedBufferTermination e)> unexpectedBufferTermination::info;
		std::stack<void(*)(unexpectedBufferTermination e)> unexpectedBufferTermination::warn;
		std::stack<int(*)(unexpectedBufferTermination e)> unexpectedBufferTermination::error;
	#endif

	//+
	//+
	//+ fatal error for when the programmer attempts to perform a non-primitive operation on a constant expression
	//+
	//+
	class nonPrimitiveOperationOnConstexpr : public fatal {
	public:
		static std::stack<void(*)(nonPrimitiveOperationOnConstexpr e)> info;
		static std::stack<void(*)(nonPrimitiveOperationOnConstexpr e)> warn;
		static std::stack<int(*)(nonPrimitiveOperationOnConstexpr e)> error;
		nonPrimitiveOperationOnConstexpr(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(nonPrimitiveOperationOnConstexpr e)> nonPrimitiveOperationOnConstexpr::info;
		std::stack<void(*)(nonPrimitiveOperationOnConstexpr e)> nonPrimitiveOperationOnConstexpr::warn;
		std::stack<int(*)(nonPrimitiveOperationOnConstexpr e)> nonPrimitiveOperationOnConstexpr::error;
	#endif

	//+
	//+
	//+ fatal error for when the programmer attempts to assign a dynamic value to a constexpr
	//+
	//+
	class dynamicAssignmentToConstexpr : public fatal {
	public:
		static std::stack<void(*)(dynamicAssignmentToConstexpr e)> info;
		static std::stack<void(*)(dynamicAssignmentToConstexpr e)> warn;
		static std::stack<int(*)(dynamicAssignmentToConstexpr e)> error;
		dynamicAssignmentToConstexpr(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		std::stack<void(*)(dynamicAssignmentToConstexpr e)> dynamicAssignmentToConstexpr::info;
		std::stack<void(*)(dynamicAssignmentToConstexpr e)> dynamicAssignmentToConstexpr::warn;
		std::stack<int(*)(dynamicAssignmentToConstexpr e)> dynamicAssignmentToConstexpr::error;
	#endif

	//+
	//+
	//+ fatal error for when a non-global variable or function is marked extern
	//+
	//+
	class nonGlobalExtern : public fatal {
	public:
		static std::stack<int(*)(nonGlobalExtern e)> error;
		static std::stack<void(*)(nonGlobalExtern e)> warn;
		static std::stack<void(*)(nonGlobalExtern e)> info;
		nonGlobalExtern(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		std::stack<int(*)(nonGlobalExtern e)> nonGlobalExtern::error;
		std::stack<void(*)(nonGlobalExtern e)> nonGlobalExtern::warn;
		std::stack<void(*)(nonGlobalExtern e)> nonGlobalExtern::info;
	#endif

	//+
	//+
	//+ fatal error for when the programmer attempts to assign a non-immediate value to a litop
	//+
	//+
	class nonImmediateLitop : public fatal {
	public:
		static std::stack<int(*)(nonImmediateLitop e)> error;
		static std::stack<void(*)(nonImmediateLitop e)> warn;
		static std::stack<void(*)(nonImmediateLitop e)> info;
		nonImmediateLitop(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		std::stack<int(*)(nonImmediateLitop e)> nonImmediateLitop::error;
		std::stack<void(*)(nonImmediateLitop e)> nonImmediateLitop::warn;
		std::stack<void(*)(nonImmediateLitop e)> nonImmediateLitop::info;
	#endif

	//+
	//+
	//+ error for when the break count is not a constant expression
	//+
	//+
	class NonConstexprBreakCount : public fatal {
	public:
		static std::stack<int(*)(NonConstexprBreakCount e)> error;
		static std::stack<void(*)(NonConstexprBreakCount e)> warn;
		static std::stack<void(*)(NonConstexprBreakCount e)> info;
		NonConstexprBreakCount(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		std::stack<int(*)(NonConstexprBreakCount e)> NonConstexprBreakCount::error;
		std::stack<void(*)(NonConstexprBreakCount e)> NonConstexprBreakCount::warn;
		std::stack<void(*)(NonConstexprBreakCount e)> NonConstexprBreakCount::info;
	#endif
//!####################################################################################################################
//!####################################################################################################################
//! ███    ██  ██████  ███    ██     ███████  █████  ████████  █████  ██          ███████ ██████  ██████   ██████  ██████  ███████
//! ████   ██ ██    ██ ████   ██     ██      ██   ██    ██    ██   ██ ██          ██      ██   ██ ██   ██ ██    ██ ██   ██ ██
//! ██ ██  ██ ██    ██ ██ ██  ██     █████   ███████    ██    ███████ ██          █████   ██████  ██████  ██    ██ ██████  ███████
//! ██  ██ ██ ██    ██ ██  ██ ██     ██      ██   ██    ██    ██   ██ ██          ██      ██   ██ ██   ██ ██    ██ ██   ██      ██
//! ██   ████  ██████  ██   ████     ██      ██   ██    ██    ██   ██ ███████     ███████ ██   ██ ██   ██  ██████  ██   ██ ███████
//!####################################################################################################################
//!####################################################################################################################
/*
 ! Non Fatal errors are issues that are errors by default but can be downgraded to warnings or outright disabled
*/

//,####################################################################################################################
//,####################################################################################################################
//, ██     ██  █████  ██████  ███    ██ ██ ███    ██  ██████  ███████
//, ██     ██ ██   ██ ██   ██ ████   ██ ██ ████   ██ ██       ██
//, ██  █  ██ ███████ ██████  ██ ██  ██ ██ ██ ██  ██ ██   ███ ███████
//, ██ ███ ██ ██   ██ ██   ██ ██  ██ ██ ██ ██  ██ ██ ██    ██      ██
//,  ███ ███  ██   ██ ██   ██ ██   ████ ██ ██   ████  ██████  ███████
//,####################################################################################################################
//,####################################################################################################################
/*
 , Warnings are issues that are warnings by default but can be disabled or upgraded to errors
*/
	class unimplementedDebugInfo : public issue {
	public:
		static action Action;
		static std::stack<void(*)(unimplementedDebugInfo e)> info;
		static std::stack<void(*)(unimplementedDebugInfo e)> warn;
		static std::stack<int(*)(unimplementedDebugInfo e)> error;
		unimplementedDebugInfo(std::string msg)
			{this->msg = msg;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		action unimplementedDebugInfo::Action = action::warning;
		std::stack<void(*)(unimplementedDebugInfo e)> unimplementedDebugInfo::info;
		std::stack<void(*)(unimplementedDebugInfo e)> unimplementedDebugInfo::warn;
		std::stack<int(*)(unimplementedDebugInfo e)> unimplementedDebugInfo::error;
	#endif

	class insufficientPrivilegeLevel : public issue {
	public:
		static action Action;
		static std::stack<void(*)(insufficientPrivilegeLevel e)> info;
		static std::stack<void(*)(insufficientPrivilegeLevel e)> warn;
		static std::stack<int(*)(insufficientPrivilegeLevel e)> error;
		insufficientPrivilegeLevel(ISSUES_CTOR_ARGS)
			{ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		action insufficientPrivilegeLevel::Action = action::warning;
		std::stack<void(*)(insufficientPrivilegeLevel e)> insufficientPrivilegeLevel::info;
		std::stack<void(*)(insufficientPrivilegeLevel e)> insufficientPrivilegeLevel::warn;
		std::stack<int(*)(insufficientPrivilegeLevel e)> insufficientPrivilegeLevel::error;
	#endif

	class stackPointerStorage : public issue {
	public:
		static action Action;
		static std::stack<void(*)(stackPointerStorage e)> info;
		static std::stack<void(*)(stackPointerStorage e)> warn;
		static std::stack<int(*)(stackPointerStorage e)> error;
		stackPointerStorage(ISSUES_CTOR_ARGS)
			{ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		action stackPointerStorage::Action = action::warning;
		std::stack<void(*)(stackPointerStorage e)> stackPointerStorage::info;
		std::stack<void(*)(stackPointerStorage e)> stackPointerStorage::warn;
		std::stack<int(*)(stackPointerStorage e)> stackPointerStorage::error;
	#endif

	class absoluteMemoryStorage : public issue {
	public:
		static action Action;
		uint64_t address;
		static std::stack<void(*)(absoluteMemoryStorage e)> info;
		static std::stack<void(*)(absoluteMemoryStorage e)> warn;
		static std::stack<int(*)(absoluteMemoryStorage e)> error;
		absoluteMemoryStorage(ISSUES_CTOR_ARGS,uint64_t address)
			:address(address){ISSUES_CTOR_INIT;invoke(*this);}
	};
	#if defined(ISSUES_CXX)
		action absoluteMemoryStorage::Action = action::warning;
		std::stack<void(*)(absoluteMemoryStorage e)> absoluteMemoryStorage::info;
		std::stack<void(*)(absoluteMemoryStorage e)> absoluteMemoryStorage::warn;
		std::stack<int(*)(absoluteMemoryStorage e)> absoluteMemoryStorage::error;
	#endif

	//,
	//,
	//, warning for when an attribute is deprecated
	//,
	//,
	class deprecatedAttribute : public issue {
	public:
		static action Action;
		static std::stack<int(*)(deprecatedAttribute e)> error;
		static std::stack<void(*)(deprecatedAttribute e)> warn;
		static std::stack<void(*)(deprecatedAttribute e)> info;
		deprecatedAttribute(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		action deprecatedAttribute::Action = action::warning;
		std::stack<int(*)(deprecatedAttribute e)> deprecatedAttribute::error;
		std::stack<void(*)(deprecatedAttribute e)> deprecatedAttribute::warn;
		std::stack<void(*)(deprecatedAttribute e)> deprecatedAttribute::info;
	#endif

	//,
	//,
	//, warning for when attribute primitiveInPlace is used before the primitve operation is specified
	//,
	//,
	class unexpectedPrimitiveInPlace : public issue {
	public:
		static action Action;
		static std::stack<int(*)(unexpectedPrimitiveInPlace e)> error;
		static std::stack<void(*)(unexpectedPrimitiveInPlace e)> warn;
		static std::stack<void(*)(unexpectedPrimitiveInPlace e)> info;
		unexpectedPrimitiveInPlace(std::string msg, origin orig, source src)
		{
			this->msg = msg;
			this->trace.push_front(orig);
			this->src = src;
			invoke(*this);
		}
	};
	#if defined(ISSUES_CXX)
		action unexpectedPrimitiveInPlace::Action = action::warning;
		std::stack<int(*)(unexpectedPrimitiveInPlace e)> unexpectedPrimitiveInPlace::error;
		std::stack<void(*)(unexpectedPrimitiveInPlace e)> unexpectedPrimitiveInPlace::warn;
		std::stack<void(*)(unexpectedPrimitiveInPlace e)> unexpectedPrimitiveInPlace::info;
	#endif

//*####################################################################################################################
//*####################################################################################################################
//* ██    ██ ██████           ██ ██████   ██████  ██     ██ ███    ██  ██████  ██████   █████  ██████  ███████
//* ██    ██ ██   ██         ██  ██   ██ ██    ██ ██     ██ ████   ██ ██       ██   ██ ██   ██ ██   ██ ██
//* ██    ██ ██████  █████  ██   ██   ██ ██    ██ ██  █  ██ ██ ██  ██ ██   ███ ██████  ███████ ██   ██ █████
//* ██    ██ ██            ██    ██   ██ ██    ██ ██ ███ ██ ██  ██ ██ ██    ██ ██   ██ ██   ██ ██   ██ ██
//*  ██████  ██           ██     ██████   ██████   ███ ███  ██   ████  ██████  ██   ██ ██   ██ ██████  ███████
//*####################################################################################################################
//*####################################################################################################################
	void changeGroupAction(std::string name, action Action);
}
