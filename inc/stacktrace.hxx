/*
 * Created Date: Friday April 12th 2024
 * Author: Lilith
 * -----
 * Last Modified: Monday June 3rd 2024 11:44:37 pm
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

#include <string>
#include <iostream>
#include <vector>
#include <execinfo.h>
#include <unistd.h>
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

inline void printStacktrace(uint64_t len)
{
	void** array = (void**)calloc(len,8);
	size_t size = backtrace(array, len);
	std::cerr << "stack trace: ";
	for(uint64_t I = 1;I<size;I++)
	{
		std::string info = get_source_info(array[I]);
		if(!info.empty())std::cerr << "\n" << info;
	}
	std::cerr << std::endl;
}
