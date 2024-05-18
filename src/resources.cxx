/*
 * Created Date: Thursday April 25th 2024
 * Author: Lilith
 * -----
 * Last Modified: Thursday April 25th 2024 2:10:06 pm
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

#include <resources.hxx>
#include <iostream>
#include <filesystem>

#define ERROR_FOPEN 0
#define ERROR_FREAD 1

static uint64_t lastFSize = 0;
static char* ReadFileContent(char* file)
{
	FILE* f = fopen(file,"r");
	if(f == nullptr)
		throw ERROR_FOPEN;
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	lastFSize = fsize;
	fseek(f, 0, SEEK_SET);  /* same as rewind(f); */
	char* data = (char*)malloc(fsize + 1);
	uint64_t out = fread(data, fsize, 1, f);
	if(out != 1)
		throw ERROR_FREAD;
	data[fsize] = 0;
	fclose(f);
	return data;
}

static std::vector<std::string> getLines(std::string path)
{
	std::vector<std::string> lines;
	try {
		char* data = ReadFileContent(path.c_str());
		uint64_t fsize = lastFSize;
		std::string working = "";
		for(uint64_t I = 0;I<fsize;I++)
		{
			if(data[I] == 0x00)break;
			if(data[I] == '\n')
			{
				lines.push_back(working);
				working = "";
			}
			else
			{
				working.push_back(data[I]);
			}
		}
		if(working != "")
		{
			lines.push_back(working);
			working = "";
		}
	} catch(int) {
		std::cerr << "could not open file: " << path << std::endl;
	}
	return lines;
}

namespace resources {
	std::map<std::string, uint64_t> ExceptionOffsets;

	void readResourceFile(std::string path)
	{
		for(std::string& sline : getLines(path))
		{
			char* line = (char*)sline.c_str();
			if(sline.length() < 4)continue;
			if(((uint32_t*)line)[0] == '-E//')
			{
				//std::cout << "exception line: " << line << std::endl;
				uint64_t start = sline.find_first_of('-',0)+1;
				uint64_t end = sline.find_last_of('-');
				std::string tname = sline.substr(start,end-start);
				std::string offset = sline.substr(end+1);
				//std::cout << "typename: " << tname << std::endl;
				//std::cout << "offset: " << offset << std::endl;
				char* fuckstdcpp = offset.c_str()+offset.length();
				uint64_t iOffset = strtoull(offset.c_str(),&fuckstdcpp,10);
				ExceptionOffsets.insert({std::pair<std::string,uint64_t>(tname,iOffset)});
			}
			else if(((uint32_t*)line)[0] == '-e//')
			{
				//std::cout << "exception line: " << line << std::endl;
				uint64_t start = 4;
				std::string offset = sline.substr(start);
				char* fuckstdcpp = offset.c_str()+offset.length();
				uint64_t iOffset = strtoull(offset.c_str(),&fuckstdcpp,10);
				nextExceptionTypeOffset = iOffset;
			}
		}
	}
}
