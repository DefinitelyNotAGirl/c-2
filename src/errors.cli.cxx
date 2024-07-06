#include <iostream>
#include <issues.hxx>
#include <colors.h>
#include <function.h>
using namespace issues;

void error_init()
{
	compilerBug::error.push(
		[](compilerBug e) -> int {
			std::cerr << COLOR_RED << "COMPILER BUG" << COLOR_RESET << ": " << e.msg << "\n";
			if(e.github == "")
			{
				std::cerr 
				<< "this issue doesn't seem to have been reported yet,\n"
				<< "please open a new issue on https://github.com/DefinitelyNotAGirl/c-2/issues and tag it as 'bug', thank you.\n"
				<< "if you could pass along the source code that triggered this bug\n"
				<< "as well as the options you ran the compiler with that would be very helpful.\n"
				;
			}
			else
			{
				std::cerr
				<< "this issue has already been reported here: " << e.github << "\n"
				<< "if you were to chime in the source code and compiler options\n"
				<< "that triggered this bug that would be very helpful.\n"
				;
			}
			e.src.print();
			e.printStackTrace();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	noSuchType::error.push(
		[](noSuchType e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": \"" << e.name << "\" does not name a type.\n";
			e.printStackTrace();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	noSuchVariable::error.push(
		[](noSuchVariable e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": \"" << e.name << "\" does not name a variable.\n";
			e.printStackTrace();
			e.src.print();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	unexpectedTokenType::error.push(
		[](unexpectedTokenType e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": unexpected " << e.src.sourceToken.Typename() << ", expected ";
			std::cerr << token::Typename(e.expectedTokenTypes.back());
			e.expectedTokenTypes.pop_back();
			while(e.expectedTokenTypes.size() > 1)
			{
				std::cerr << "," << token::Typename(e.expectedTokenTypes.back());
				e.expectedTokenTypes.pop_back();
			}
			if(e.expectedTokenTypes.size() == 1)
			{
				std::cerr << " or " << token::Typename(e.expectedTokenTypes.back());
			}
			std::cerr << std::endl;
			e.printStackTrace();
			e.src.print();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	noSuchIdentifier::error.push(
		[](noSuchIdentifier e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": unresolved identifier \"" << e.name << "\"\n";
			e.printStackTrace();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	noSuchLitop::error.push(
		[](noSuchLitop e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": no such Literal operator \"" << e.name << "\"\n";
			e.printStackTrace();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	noSuchNumberSystem::error.push(
		[](noSuchNumberSystem e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": no such number system \"" << e.name << "\"\n";
			e.printStackTrace();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	noSuchFunction::error.push(
		[](noSuchFunction e) -> int {
			std::string neededExpression = e.neededFunction->expression_ansi();
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": no such function: " << neededExpression << "\n";
			if(e.candidates.size() > 0)
			{
				std::cerr << "candidates: \n";
				for(function* candidate : e.candidates)
					std::cerr << "    " << candidate->expression_ansi() << "\n";
			}
			e.printStackTrace();
			std::cerr << "\r\n" << std::endl;
			ErrorCount++;
			return 1;
		}
	);
	noSuchABI::error.push(
		[](noSuchABI e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": \"" << e.name << "\" does not name an ABI.\n";
			e.printStackTrace();
			e.src.print();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	nonImmediateLitop::error.push(
		[](nonImmediateLitop e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": value for litop \"" << e.msg << "\" is not an immediate.\n";
			e.printStackTrace();
			e.src.print();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
	invalidAttribute::error.push(
		[](invalidAttribute e) -> int {
			std::cerr << COLOR_RED << "ERROR" << COLOR_RESET << ": invalid attribute \"" << e.attribute << "\" for "<<e.entityName<<"\n";
			std::cout << "valid attributes:\n";
			for(std::string a : e.validAttributes)
				std::cout << "    " << a << "\n";
			e.printStackTrace();
			e.src.print();
			std::cerr << "\n\n\r";
			ErrorCount++;
			return 1;
		}
	);
}