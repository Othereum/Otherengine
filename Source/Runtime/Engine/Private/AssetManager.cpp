#include "AssetManager.hpp"
#include "Engine.hpp"

namespace logcat
{
const LogCategory kAsset{u8"Asset"sv};
}

namespace oeng
{
inline namespace engine
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

std::shared_ptr<Object> AssetManager::Load(Path path)
{
    if (const auto found = assets_.find(path); found != assets_.end())
    {
        if (auto ptr = found->second.lock())
            return ptr;

#ifndef NDEBUG
        ++reload_count_[path];
#endif
    }

    const auto json = ReadJsonFile(path);
    const auto type = json.at("type").get<Name>();

    auto loaded = Factory::Get().Create(type);
    loaded->from_json(json);

    assets_.insert_or_assign(path, loaded);
    return loaded;
}
}
}
