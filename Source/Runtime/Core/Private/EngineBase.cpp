#include "EngineBase.hpp"
#include <csignal>

namespace logcat
{
const LogCategory kEngine{u8"Engine"sv};
}

namespace oeng
{
inline namespace core
{
static EngineBase* engine_base = nullptr;

RegisterEngineBase::RegisterEngineBase()
{

    assert(!engine_base);
    engine_base = reinterpret_cast<EngineBase*>(this);
}

RegisterEngineBase::~RegisterEngineBase()
{
    engine_base = nullptr;
}

EngineBase::EngineBase(std::u8string game_module_path)
    : game_dll{std::move(game_module_path)},
      game_name_{game_dll.GetSymbol<const char8_t* const>(u8"kGameName"sv)}
{
}
}
}
