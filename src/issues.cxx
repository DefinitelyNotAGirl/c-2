#include <issues.hxx>
using namespace issues;

//+####################################################################################################################
//+####################################################################################################################
//+ ██████  ███████ ███████ ██ ███    ██ ███████      █████   ██████ ████████ ██  ██████  ███    ██ ███████
//+ ██   ██ ██      ██      ██ ████   ██ ██          ██   ██ ██         ██    ██ ██    ██ ████   ██ ██
//+ ██   ██ █████   █████   ██ ██ ██  ██ █████       ███████ ██         ██    ██ ██    ██ ██ ██  ██ ███████
//+ ██   ██ ██      ██      ██ ██  ██ ██ ██          ██   ██ ██         ██    ██ ██    ██ ██  ██ ██      ██
//+ ██████  ███████ ██      ██ ██   ████ ███████     ██   ██  ██████    ██    ██  ██████  ██   ████ ███████
//+####################################################################################################################
//+####################################################################################################################
action unimplementedDebugInfo::Action = action::warning;

//+####################################################################################################################
//+####################################################################################################################
//+ ██████  ███████ ███████ ██ ███    ██ ███████     ██   ██  █████  ███    ██ ██████  ██      ███████
//+ ██   ██ ██      ██      ██ ████   ██ ██          ██   ██ ██   ██ ████   ██ ██   ██ ██      ██
//+ ██   ██ █████   █████   ██ ██ ██  ██ █████       ███████ ███████ ██ ██  ██ ██   ██ ██      █████
//+ ██   ██ ██      ██      ██ ██  ██ ██ ██          ██   ██ ██   ██ ██  ██ ██ ██   ██ ██      ██
//+ ██████  ███████ ██      ██ ██   ████ ███████     ██   ██ ██   ██ ██   ████ ██████  ███████ ███████
//+####################################################################################################################
//+####################################################################################################################
std::stack<void(*)(compilerBug e)> compilerBug::handle;
std::stack<void(*)(unexpectedTokenType e)> unexpectedTokenType::handle;
std::stack<void(*)(noSuchFile e)> noSuchFile::handle;
std::stack<void(*)(noSuchType e)> noSuchType::handle;
std::stack<void(*)(noSuchIdentifier e)> noSuchIdentifier::handle;
std::stack<void(*)(invalidUseOfKeywordInScope e)> invalidUseOfKeywordInScope::handle;
std::stack<void(*)(noSuchABI e)> noSuchABI::handle;
std::stack<void(*)(nonImmediateArraySize e)> nonImmediateArraySize::handle;
std::stack<void(*)(nonImmediateIntegerTemplateArgument e)> nonImmediateIntegerTemplateArgument::handle;
std::stack<void(*)(noSuchLitop e)> noSuchLitop::handle;
std::stack<void(*)(invalidAttribute e)> invalidAttribute::handle;
std::stack<void(*)(noSuchFunction e)> noSuchFunction::handle;
std::stack<void(*)(noSuchVariable e)> noSuchVariable::handle;
std::stack<void(*)(noSuchMangler e)> noSuchMangler::handle;
std::stack<void(*)(noSuchSystem e)> noSuchSystem::handle;
std::stack<void(*)(noSuchArchitecture e)> noSuchArchitecture::handle;
std::stack<void(*)(noSuchNumberSystem e)> noSuchNumberSystem::handle;
std::stack<void(*)(invalidType e)> invalidType::handle;
std::stack<void(*)(unexpectedBufferTermination e)> unexpectedBufferTermination::handle;
std::stack<void(*)(unimplementedDebugInfo e)> unimplementedDebugInfo::handle;
