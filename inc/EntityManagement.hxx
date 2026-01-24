#pragma once

#include "mangling.h"
#include "ABI.h"
#include "class_variable.h"
#include "class_type.h"

variable* call(function* func,std::vector<variable*> args);

namespace Entity {
	enum class AttributeType {
		Invalid,
		Mangling,
		Local,
		Volatile,
		NoAlloc,
		Inline,
		Const,
		Constexpr,
		Extern,
		Noreturn,
		Typecast,
		ImplicitCast,
		ExplicitCast,
		Noop,
		Deprecated,
		ABI,
		Symbol,
		Primitive,
		Accessibility,
		AbsoluteMemoryStorage,
		StackStorage,
		RegisterStorage
	};
	extern std::stack<std::string> ExceptionHandlers;
	struct PrimitiveAttributeData {
		primitiveOP Operation;
		bool InPlace;
		PrimitiveAttributeData(){}
		PrimitiveAttributeData(primitiveOP Operation,bool InPlace = false)
			:Operation(Operation),InPlace(InPlace){}
	};
	class Attribute {
	private:
		void init();
	public:
		AttributeType Type;
		token Token;
		//, these were supposed to be a union but C++ is being a piece of shit so i guess they wont be...
		//, "... constructor deleted ... ill-formed ..." - my ass, 
		//, calling the constructors of union members is totally illogical and as a consequence a union can't have a constructor.
		//, do not attempt to refactor this into a union, total hours wasted here: 1
		//, - Lilith
		PrimitiveAttributeData Primitive;
		mangler* Mangler;
		ABI* Abi;
		std::string Symbol;
		vaccess Accessibility;
		int64_t StackOffset;
		uint64_t Address;
		std::string Register;

		Attribute(AttributeType Type)
			:Type(Type){}

		Attribute(PrimitiveAttributeData data)
			:Primitive(data),Type(AttributeType::Primitive){}

		Attribute(mangler* Mangler)
			:Mangler(Mangler),Type(AttributeType::Mangling){}

		Attribute(ABI* Abi)
			:Abi(Abi),Type(AttributeType::ABI){}

		Attribute(uint64_t Address)
			:Address(Address),Type(AttributeType::AbsoluteMemoryStorage){}

		Attribute(int64_t StackOffset)
			:StackOffset(StackOffset),Type(AttributeType::StackStorage){}

		Attribute(std::string data, AttributeType Type)
			:Register(data),Symbol(data),Type(Type){}

		Attribute(vaccess Accessibility)
			:Accessibility(Accessibility),Type(AttributeType::Accessibility){}
	};

	function* startFunctionDefinition(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType, bool isIndentBased);
	function* declareFunction(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType);

	variable* defineVariable(std::vector<Attribute>& attributes,std::string& name, type* Type);

	type* startTypeDefinition(std::vector<Attribute>& attributes, std::string& name, std::vector<token>& inherit, bool isIndentBased);
	type* declareType(std::vector<Attribute>& attributes, std::string name, std::vector<token> inherit);

	bool close(scope* s);

	/**
	 * @brief updates the compilers current operating scope, this should really be reafactored into a stack, 
	 * but it's deeply embedded into the code so refactoring would be a pain in the ass
	 */
	inline void updateCurrentScope(scope* sc) {
		currentScope = sc;
		if(sc->func != nullptr)
			code = sc->func->code;
	}
}