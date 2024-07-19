#include "../parser.hxx"

#include <filesystem>

extern std::vector<line> getLines(std::string fname);

void parse::Directives::Include()
{
	ParserState.Token = ParserState.Line.nextToken();
	switch (ParserState.Token.type) {
		case (6):{ // default width string literal
			// std::cout << "including file: " << ParserState.Token.text <<
			// std::endl;
			std::string inc = ParserState.Token.text;
			if (std::filesystem::exists(inc)) {
				std::string rstFile = currentFile;
				currentFile = inc;
				auto Lines = getLines(inc);
				parse::Lines(Lines);
				currentFile = rstFile;
				//resetScope();
				includedFiles.push_back(inc);
				compilerBug("unimplemented: debug file information");
			}
			else
				noSuchFile("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),inc,{});
			break;
		}
		case (34):
			std::list<std::string> includePathsChecked;
			ParserState.Token = ParserState.Line.nextToken();
			//std::string fname =
			//	ParserState.Token.texParserState.Token.substr(1, ParserState.Token.texParserState.Token.length() - 2);
			std::string fname = ParserState.Token.text;
			// std::cout << "sys include: " << fname <<
			// std::endl;
			std::string inc;
			for (std::string i : includeDirs) {
				inc = i + "/" + fname;
				if (std::filesystem::exists(inc)) {
					std::string rstFile = currentFile;
					currentFile = inc;
					auto Lines = getLines(inc);
					parse::Lines(Lines);
					currentFile = rstFile;
					//resetScope();
					includedFiles.push_back(inc);
					dependencies.push_back(inc);
					goto sysIncludeSuccess;
				}
				else
					includePathsChecked.push_back(inc);
				inc += ".h2";
				if (std::filesystem::exists(inc)) {
					std::string rstFile = currentFile;
					currentFile = inc;
					auto Lines = getLines(inc);
					parse::Lines(Lines);
					currentFile = rstFile;
					//resetScope();
					includedFiles.push_back(inc);
					dependencies.push_back(inc);
					goto sysIncludeSuccess;
				}
				else
					includePathsChecked.push_back(inc);
			}
			noSuchFile("",originCoreHere,source(currentFile,ParserState.Line,ParserState.Token),inc,includePathsChecked);
			sysIncludeSuccess:;
			unimplementedDebugInfo("debug file information");
			if(options::ddebug)
				std::cout << "included file: " << inc << std::endl;
			break;
	}
}
