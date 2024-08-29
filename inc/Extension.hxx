#include <string>
#include <map>

typedef void(*ExtensionCallback)();
extern std::map<std::string,ExtensionCallback> ExtensionCallbacks;
