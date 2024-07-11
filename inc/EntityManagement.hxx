#pragma once

#include <mangling.h>
#include <ABI.h>
#include <class_variable.h>

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
	struct PrimitiveAttributeData {
		primitiveOP Operation;
		bool InPlace;
		PrimitiveAttributeData(primitiveOP Operation,bool InPlace = false)
			:Operation(Operation),InPlace(InPlace){}
	};
	class Attribute {
	private:
		void init();
	public:
		AttributeType Type;
		token Token;
		union {
			PrimitiveAttributeData Primitive;
			mangler* Mangler;
			ABI* Abi;
			std::string Symbol;
			vaccess Accessibility;
			int64_t StackOffset;
			uint64_t Address;
			std::string Register;
		private:
			std::string strData;
		};

		Attribute(AttributeType Type)
			:Type(Type){}

		Attribute(PrimitiveAttributeData data)
			:Primitive(data),Type(AttributeType::Primitive){}

		Attribute(mangler* Mangler)
			:Mangler(Mangler),Type(AttributeType::Mangling){}

		Attribute(ABI* Abi)
			:Abi(Abi),Type(AttributeType::ABI){}

		Attribute(std::string& data, AttributeType Type)
			:strData(data),Type(Type){}

		Attribute(vaccess Accessibility)
			:Accessibility(Accessibility),Type(AttributeType::Accessibility){}
	};

	function* startFunctionDefinition(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType, bool isIndentBased);
	function* declareFunction(std::vector<Attribute>& attributes,std::string& name, std::vector<variable*>& args, type* returnType);

	variable* defineVariable(std::vector<Attribute>& attributes,std::string& name, type* Type)

	bool close(scope* s);
}