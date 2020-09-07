#pragma once

namespace logcat
{
extern ENGINE_API const LogCategory kAsset;
}

namespace oeng
{
inline namespace engine
{
class ENGINE_API AssetManager
{
public:
    [[nodiscard]] static AssetManager& Get();

    /**
     * Returns the asset if it is already loaded, otherwise load it.
     * @param path Asset path.
     * @return Loaded asset.
     */
    [[nodiscard]] std::shared_ptr<Object> Load(Path path);

    /**
     * Returns the asset if it is already loaded, otherwise load it.
     * @tparam T Asset type.
     * @param path Asset path.
     * @return Loaded asset.
     * @throw std::bad_cast If failed to cast from actual type of asset to type `T`.
     */
    template <class T>
    [[nodiscard]] std::shared_ptr<T> Load(Path path)
    {
        return Cast<T>(Load(path));
    }

#ifndef NDEBUG
    void LogReloadCount();
#endif

private:
    AssetManager() = default;

    std::unordered_map<Path, std::weak_ptr<Object>> assets_{};
#ifndef NDEBUG
    std::unordered_map<Path, size_t> reload_count_{};
#endif
};
}
}
