#pragma once
#include "Object/Object.hpp"
#include "Serialization/ArchiveFileReader.hpp"

namespace logcat
{
extern ENGINE_API const LogCategory kAsset;
}

namespace oeng::engine
{
class ENGINE_API AssetManager
{
public:
    DELETE_CPMV(AssetManager);
    ~AssetManager();

    [[nodiscard]] static AssetManager& Get() noexcept;

    /**
     * Returns the asset if it is already loaded, otherwise load it.
     * @tparam T Asset type. Must match actual type.
     * @param path Asset path.
     * @return Loaded asset.
     * @throw std::bad_cast If failed to cast from actual type of asset to type `T`.
     * @throw std::exception Any exceptions thrown from the constructor of type `T`.
     */
    template <class T>
    [[nodiscard]] std::enable_if_t<std::convertible_to<T*, Object*>, std::shared_ptr<T>> Load(Path path)
    {
        if (const auto found = assets_.find(path); found != assets_.end())
        {
            if (const auto ptr = found->second.lock())
                return {std::move(ptr), &dynamic_cast<T&>(*ptr)};

#ifndef NDEBUG
            ++reload_count_[path];
#endif
        }

        ArchiveFileReader file{path};
        Name class_name;
        file << class_name;

        auto loaded = Factory::Get().Create<T>(class_name);
        file << *loaded;

        assets_.insert_or_assign(path, loaded);
        return loaded;
    }

private:
    std::unordered_map<Path, std::weak_ptr<Object>> assets_;
#ifndef NDEBUG
    std::unordered_map<Path, unsigned> reload_count_;
#endif
};
}
