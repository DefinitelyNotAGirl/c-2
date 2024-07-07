#include "../parser.hxx"

void parse::Directives::Autodecl()
{
	ParserState.Token = ParserState.Token.nextToken();
	if (ParserState.Token.text == "integer") {
		// name
		ParserState.Token				 = ParserState.Token.nextToken();
		std::string name = ParserState.Token.text;
		// size
		ParserState.Token			  = ParserState.Token.nextToken();
		uint64_t size = std::stoi(ParserState.Token.text);
		std::vector<line> gLines;
		line l;
		l	   = ParserState.Token;
		l.tpos = 0;
		if (getType(name) == nullptr) {
			switch (size) {
				case (0):
					l.text = ParserState.Token.restText() + " class " + name +
							 " extends primitive0;";
					gLines.push_back(l);
					break;
				case (1):
					l.text = ParserState.Token.restText() + " class " + name +
							 " extends primitive8;";
					gLines.push_back(l);
					break;
				case (2):
					l.text = ParserState.Token.restText() + " class " + name +
							 " extends primitive16;";
					gLines.push_back(l);
					break;
				case (4):
					l.text = ParserState.Token.restText() + " class " + name +
							 " extends primitive32;";
					gLines.push_back(l);
					break;
				case (8):
					l.text = ParserState.Token.restText() + " class " + name +
							 " extends primitive64;";
					gLines.push_back(l);
					break;
			}
	}
		if (size > 0) {
			if(defaultUnsignedIntegerType != nullptr){
				if(defaultUnsignedIntegerType->name != name){
					l.text = ParserState.Token.restText() + " primitiveAssign primitiveInPlace void operator=(" + name + "," + defaultUnsignedIntegerType->name + ");";
					gLines.push_back(l);
					//std::cout << "gline: " << l.text << std::endl;
				}
				//else
				//{
				//    std::cout << "\""<<name<<"\" == \"" << defaultUnsignedIntegerType->name << "\"" << std::endl;
				//}
			}
			else
			{
				warn(getWarning("autodecl-nodef-uint"),&ParserState.Token,"\"#autodecl integer\" used before assigning a default unsigned integer type.");
			}
			l.text = ParserState.Token.restText() + " primitiveMul " + name +
					 " operator*(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() + " primitiveDiv " + name +
					 " operator/(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() + " primitiveAdd " + name +
					 " operator+(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() + " primitiveSub " + name +
					 " operator-(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() + " primitiveMod " + name +
					 " operator%(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveMul primitiveInPlace void "
					 "operator*=(" +
					 name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveDiv primitiveInPlace void "
					 "operator/=(" +
					 name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveAdd primitiveInPlace void "
					 "operator+=(" +
					 name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveSub primitiveInPlace void "
					 "operator-=(" +
					 name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveMod primitiveInPlace void "
					 "operator%=(" +
					 name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +" primitiveEqual "+defaultBooleanType->name+" operator==(" + name +"," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +" primitiveGreater "+defaultBooleanType->name+" operator>(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +" primitiveGreaterEqual "+defaultBooleanType->name+" operator>=(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +" primitiveLessEqual "+defaultBooleanType->name+" operator<=(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +" primitiveLess "+defaultBooleanType->name+" operator<(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +" primitiveNotEqual "+defaultBooleanType->name+" operator!=(" + name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveAssign primitiveInPlace void "
					 "operator=(" +
					 name + "," + name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveInc primitiveInPlace void "
					 "operator++(" +
					 name + ");";
			gLines.push_back(l);
			l.text = ParserState.Token.restText() +
					 " primitiveDec primitiveInPlace void "
					 "operator--(" +
					 name + ");";
			gLines.push_back(l);
			if(name != defaultUnsignedIntegerType->name)
			{
					l.text = ParserState.Token.restText() + " primitiveMul " + name +
						 " operator*(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() + " primitiveDiv " + name +
						 " operator/(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() + " primitiveAdd " + name +
						 " operator+(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() + " primitiveSub " + name +
						 " operator-(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() + " primitiveMod " + name +
						 " operator%(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +
						 " primitiveMul primitiveInPlace void "
						 "operator*=(" +
						 name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +
						 " primitiveDiv primitiveInPlace void "
						 "operator/=(" +
						 name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +
						 " primitiveAdd primitiveInPlace void "
						 "operator+=(" +
						 name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +
						 " primitiveSub primitiveInPlace void "
						 "operator-=(" +
						 name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +
						 " primitiveMod primitiveInPlace void "
						 "operator%=(" +
						 name + ","+defaultUnsignedIntegerType->name+");";
				l.text = ParserState.Token.restText() +" primitiveEqual "+defaultBooleanType->name+" operator==(" + name +","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +" primitiveGreater "+defaultBooleanType->name+" operator>(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +" primitiveGreaterEqual "+defaultBooleanType->name+" operator>=(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +" primitiveLessEqual "+defaultBooleanType->name+" operator<=(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +" primitiveLess "+defaultBooleanType->name+" operator<(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
				l.text = ParserState.Token.restText() +" primitiveNotEqual "+defaultBooleanType->name+" operator!=(" + name + ","+defaultUnsignedIntegerType->name+");";
				gLines.push_back(l);
			}
		}
		if (gLines.size() > 0) parse(gLines);
	}
}
