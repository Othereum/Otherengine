#include "AssetManager.hpp"
#include "Engine.hpp"

namespace logcat
{
const LogCategory kAsset{u8"Asset"sv};
}

namespace oeng::engine
{
AssetManager::~AssetManager()
#ifndef NDEBUG
{
    if (!reload_count_.empty())
    {
        OE_LOG(kAsset, kDebug, u8"Asset reload count"sv);

        for (const auto& [path, count] : reload_count_)
        {
            OE_LOG(kAsset, kDebug, u8"{}: {} times"sv, path.Str(), count);
        }
    }
}
#else
= default;
#endif

AssetManager& AssetManager::Get() noexcept
{
    return Engine::Get().GetAssetManager();
}
}
