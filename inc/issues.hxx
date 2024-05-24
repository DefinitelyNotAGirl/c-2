/*
 * Created Date: Wednesday May 22nd 2024
 * Author: Lilith
 * -----
 * Last Modified: Wednesday May 22nd 2024 11:38:05 am
 * Modified By: Lilith (definitelynotagirl115169@gmail.com)
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

#include <compiler.h>
#include <colors.h>
#include <stacktrace.hxx>

#include <backtrace.h>
#include <cxxabi.h> // For __cxa_demangle


struct TraceInfo {
    std::string file;
    int line;
    std::string function;
};

static void error_callback(void *data, const char *msg, int errnum) {
    std::cerr << "Error: " << msg << " (errnum " << errnum << ")\n";
}

static void syminfo_callback(void *data, uintptr_t pc, const char *symname, uintptr_t symval, uintptr_t symsize) {
    TraceInfo *info = reinterpret_cast<TraceInfo*>(data);
    if (symname) {
        int status;
        char *demangled = abi::__cxa_demangle(symname, nullptr, nullptr, &status);
        if (status == 0) {
            info->function = demangled;
            free(demangled);
        } else {
            // Demangling failed, use the original symbol name
            info->function = symname;
        }
    }
}

static int full_callback(void *data, uintptr_t pc, const char *filename, int lineno, const char *function) {
    auto trace_info = reinterpret_cast<std::vector<TraceInfo>*>(data);
    TraceInfo info;
    if (filename) {
        info.file = filename;
        info.line = lineno;
    }
    if (function) {
        int status;
        char *demangled = abi::__cxa_demangle(function, nullptr, nullptr, &status);
        if (status == 0) {
            info.function = demangled;
            free(demangled);
        } else {
            // Demangling failed, use the original function name
            info.function = function;
        }
    }
    trace_info->push_back(info);
    return 0;
}

static std::string get_source_info(void *addr) {
    backtrace_state *state = backtrace_create_state(nullptr, 1, error_callback, nullptr);
    if (!state) {
        return "Failed to create backtrace state.";
    }

    std::vector<TraceInfo> trace_info;
    backtrace_pcinfo(state, reinterpret_cast<uintptr_t>(addr), full_callback, error_callback, &trace_info);
    if (trace_info.empty()) {
        return "No source info available.";
    }

    const TraceInfo &info = trace_info.front();
    std::string result;
    if (!info.file.empty()) {
        result += info.file + ":" + std::to_string(info.line);
    }
    if (!info.function.empty()) {
        result += " in " + info.function;
    }
    return result;
}

namespace issues {
	#define originCoreHere origin("core",__FILE__,__LINE__)

	enum class action {
		error,
		warning,
		ignore
	};

	template<typename issueType>
	void invoke(issueType e)
	{
		switch(e.Action)
		{
			case(action::error):
			{
				throw e;
			}
			case(action::warning):
			{
				e.warn();
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

	class fatal : public issue {
	public:
		const static action Action = action::error;
		void warn(){}
	};

	#define ISSUES_CTOR_ARGS std::string msg, origin orig, source src
	#define ISSUES_CTOR_INIT this->msg = msg;this->trace.push_front(orig);this->src = src

	class compilerBug : public fatal {
	public:
		std::string github; //link to issue, empty string if no issue exists
		compilerBug(ISSUES_CTOR_ARGS, std::string github)
			:github(github){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class unexpectedTokenType : public fatal {
	public:
		std::list<uint64_t> expectedTokenTypes;
		unexpectedTokenType(ISSUES_CTOR_ARGS, std::list<uint64_t> expectedTokenTypes)
			:expectedTokenTypes(expectedTokenTypes){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchFile : public fatal {
	public:
		std::string file;//examples
						 //		"blub.h2"
						 //		<blub.h2>
		std::list<std::string> checkedPaths;
		noSuchFile(ISSUES_CTOR_ARGS, std::string file, std::list<std::string> checkedPaths)
			:file(file),checkedPaths(checkedPaths){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchType : public fatal {
	public:
		std::string name;
		noSuchType(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchIdentifier : public fatal {
	public:
		std::string name;
		noSuchIdentifier(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class invalidUseOfKeywordInScope : public fatal {
	public:
		scope* Scope;
		invalidUseOfKeywordInScope(ISSUES_CTOR_ARGS, scope* Scope)
			:Scope(Scope){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchABI : public fatal {
	public:
		std::string name;
		noSuchABI(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class nonImmediateArraySize : public fatal {
	public:
		type* valueType;
		nonImmediateArraySize(ISSUES_CTOR_ARGS, type* valueType)
			:valueType(valueType){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchLitop : public fatal {
	public:
		std::string name;
		noSuchLitop(ISSUES_CTOR_ARGS, std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class invalidAttribute : public fatal {
	public:
		std::string entityName;
		std::string attribute;
		std::list<std::string> validAttributes;
		invalidAttribute(ISSUES_CTOR_ARGS, std::string entityName,std::string attribute,std::list<std::string> validAttributes)
			:entityName(entityName),attribute(attribute),validAttributes(validAttributes){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchFunction : public fatal {
	public:
		function* neededFunction;
		std::vector<function*> candidates;
		noSuchFunction(ISSUES_CTOR_ARGS,function* neededFunction, std::vector<function*> candidates)
			:neededFunction(neededFunction),candidates(candidates){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchVariable : public fatal {
	public:
		std::string name;
		noSuchVariable(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchMangler : public fatal {
	public:
		std::string name;
		noSuchMangler(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchSystem : public fatal {
	public:
		std::string name;
		noSuchSystem(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class noSuchArchitecture : public fatal {
	public:
		std::string name;
		noSuchArchitecture(ISSUES_CTOR_ARGS,std::string name)
			:name(name){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class invalidType : public fatal {
	public:
		std::list<type*> validTypes;
		type* receivedType;
		invalidType(ISSUES_CTOR_ARGS,std::list<type*> validTypes,type* receivedType)
			:validTypes(validTypes),receivedType(receivedType){ISSUES_CTOR_INIT;invoke(*this);}
	};

	class unexpectedBufferTermination : public fatal {
	public:
		unexpectedBufferTermination(ISSUES_CTOR_ARGS)
			{ISSUES_CTOR_INIT;invoke(*this);}
	};
}
