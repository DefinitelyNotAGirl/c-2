#include <string>
#include <vector>
#include "extint.hxx"
#include "class_token.h"

template<typename EventData_T>
class EventHandler {
public:
	typedef void(*EventHandlerCallback)(void* HandlerData,EventData_T* EventData);
	void* HandlerData = nullptr;
	EventHandlerCallback callback;

	EventHandler(void* HandlerData,EventHandlerCallback callback)
		:callback(callback),HandlerData(HandlerData){}

	EventHandler(EventHandlerCallback callback)
		:callback(callback){}

	void run(EventData_T* EventData){
		this->callback(this->HandlerData,EventData);
	}
};

template<typename EventData_T>
class Event_T {
public:
	std::vector<EventHandler<EventData_T>> EventHandlers;
	void fire(EventData_T* EventData) {
		for(EventHandler<EventData_T>& handler : this->EventHandlers) {
			handler.run(EventData);
		}
	}
};

#ifndef COCK
	#define COCK extern
#endif

//.
//. get data types
//.
class variable;
class function;
class type;

//.
//.	events
//.
namespace Event {
	namespace Data {
		class FunctionDeclaration {
		public:
			function* func;
		};
	}
	/**
	 * @brief Fired when a function is declared
	 */
	COCK Event_T<Data::FunctionDeclaration> FunctionDeclaration;
	namespace Data {
		class FunctionSwitch {
		public:
			function* func;
		};
	}
	/**
	 * @brief Fired when the function for which the compiler is generating code changes. This event does not imply that the function is done.
	 */
	COCK Event_T<Data::FunctionSwitch> FunctionSwitch;
	namespace Data {
		class VariableDeclaration {
		public:
			variable* var;
		};
	}
	/**
	 * @brief Fired when a variable is declared
	 */
	COCK Event_T<Data::VariableDeclaration> VariableDeclaration;
	namespace Data {
		class TypeDeclaration {
		public:
			type* Type;
		};
	}
	/**
	 * @brief Fired when a type is declared
	 */
	COCK Event_T<Data::TypeDeclaration> TypeDeclaration;
	namespace Data {
		class BranchSplit {
		public:
			std::string condition;
		};
	}
	/**
	 * @brief Fired when the current branch splits
	 */
	COCK Event_T<Data::BranchSplit> BranchSplit;
	namespace Data {
		class BranchSwitch {
		public:
			u64 BranchID;
		};
	}
	/**
	 * @brief Fired when the compiler switches the branch for which it is generating code
	 */
	COCK Event_T<Data::BranchSwitch> BranchSwitch;
	namespace Data {
		class BranchExit {
		public:
		};
	}
	/**
	 * @brief Fired when the current branch exists
	 */
	COCK Event_T<Data::BranchExit> BranchExit;
	namespace Data {
		class BranchMerge {
		public:
			/**
			 * @brief The current branch merges with TargetBranch
			 */
			u64 TargetBranch;
		};
	}
	/**
	 * @brief Fired when the current branch merges with another
	 */
	COCK Event_T<Data::BranchMerge> BranchMerge;
	namespace Data {
		class Call {
		public:
			function* func;
			std::vector<variable*>* args;
		};
	}
	/**
	 * @brief Fired when a function is called, applies to primitive functions
	 */
	COCK Event_T<Data::Call> Call;
	namespace Data {
		class DestroyVariable {
		public:
			variable* var;
		};
	}
	/**
	 * @brief Fired when a variables constructor is ran. If the destructor is a non-primitive function a seperate Call event is Fired.
	 */
	COCK Event_T<Data::DestroyVariable> DestroyVariable;
	namespace Data {
		class TokenIdentified {
		public:
			token* Token;
			void* obj;
		};
	}
	/**
	 * @brief fired when a token is itendtified
	 */
	COCK Event_T<Data::TokenIdentified> TokenIdentified;
	namespace Data {
		class FunctionImplementation {
		public:
			function* func;
		};
	}
	/**
	 * @brief fired when a function body starts
	 */
	COCK Event_T<Data::FunctionImplementation> FunctionImplementation;
	namespace Data {
		class FunctionFinalization {
		public:
			function* func;
		};
	}
	/**
	 * @brief fired when a function finalizes
	 */
	COCK Event_T<Data::FunctionFinalization> FunctionFinalization;
	namespace Data {
		class TypeFinalization {
		public:
			type* Type;
		};
	}
	/**
	 * @brief fired when a type is finalized
	 */
	COCK Event_T<Data::TypeFinalization> TypeFinalization;
}
