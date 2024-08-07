#include <Routine.hxx>
#include <vector>
extern std::vector<Routine> PostCLIRoutines;

#include <options.h>
#include <event.hxx>
#include <Parser.hxx>
extern std::string __reqFileVSTC;
inline void LoadPostCLI(void*) {
	if(!options::vstc)return;
	Event::TokenIdentified.EventHandlers.push_back(
		EventHandler<Event::Data::TokenIdentified>(
			[](void* HandlerData, Event::Data::TokenIdentified* EventData) {
				if(ParserState.File != __reqFileVSTC)return;
				//std::cout << ParserState.File << " == " << __reqFileVSTC << std::endl;
				//std::cout << "Token identified: " << EventData->Token->text << std::endl;
				std::string mdata;
				std::string ID = "0000";
				switch(EventData->Token->type) {
					case(9):
						//typename
						ID="0001";
						mdata = "\x20"+((type*)EventData->obj)->__declared_file+"\x20"+std::to_string(((type*)EventData->obj)->__declared_line)+"\x20"+((type*)EventData->obj)->desc;
						break;
					case(10):
						//variable name
						ID="0002";
						mdata = "\x20"+((variable*)EventData->obj)->__declared_file+"\x20"+std::to_string(((variable*)EventData->obj)->__declared_line)+"\x20"+((variable*)EventData->obj)->dataType->name+"\x20"+((variable*)EventData->obj)->desc;
						break;
					case(11):
						//function name
						ID="0003";
						mdata = "\x20"+((function*)EventData->obj)->__declared_file+"\x20"+std::to_string(((function*)EventData->obj)->__declared_line)+"\x20"+((function*)EventData->obj)->returnType->name+"\x20"+((function*)EventData->obj)->desc+"\x20"+((function*)EventData->obj)->returnDesc;
						break;
					case(60):
						//parameter name
						ID="0004";
						mdata = "\x20"+((variable*)EventData->obj)->__declared_file+"\x20"+std::to_string(((variable*)EventData->obj)->__declared_line)+"\x20"+((variable*)EventData->obj)->dataType->name;
						break;
					case(61):
						//integer
						ID="0005";
						mdata = "\x20"+std::to_string((u64)EventData->obj)+"\x20"+"dummy";
						break;
					default: {
						return;
					}
				}
				//if(EventData->Token->lineNum == 85) {
				//	std::cout << "85" << std::endl;
				//}
				std::cout << ID << '\x20' << EventData->Token->lineNum <<'\x20'<< EventData->Token->tcol <<'\x20'<< EventData->Token->text.length() <<'\x20'<<EventData->Token->text<<mdata<< '\n';
			}
		)
	);
}
__attribute__((constructor)) static void init(){PostCLIRoutines.push_back(Routine(nullptr,&LoadPostCLI));}