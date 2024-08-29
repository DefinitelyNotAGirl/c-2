#include "parser.hxx"
#include <Extension.hxx>

void parse::Descriptor() {
	try {
		ExtensionCallbacks.at(ParserState.Token.text.substr(1))();
	}catch(std::out_of_range){}
}
