#pragma once
#include "Serialization/ArchiveFileReader.hpp"
#include "Serialization/Serializable.hpp"

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
     * @throw std::bad_cast If the type `T` does not match the type of already loaded asset.
     * @throw std::exception Any exceptions thrown from the constructor of type `T`.
     */
    template <std::derived_from<ISerializable> T>
    [[nodiscard]] SharedRef<T> Load(Path path)
    {
        if (const auto found = assets_.find(path); found != assets_.end())
        {
            if (const auto ptr = found->second.lock())
                return DynamicCast<T>(SharedRef{std::move(ptr)});

#ifndef NDEBUG
            ++reload_count_[path];
#endif
        }

        ArchiveFileReader file{path};
        auto loaded = MakeShared<T>();
        file << *loaded;

        assets_.insert_or_assign(path, loaded);
        return loaded;
    }

private:
    std::unordered_map<Path, WeakPtr<ISerializable>> assets_;
#ifndef NDEBUG
    std::unordered_map<Path, unsigned> reload_count_;
#endif
};
}
