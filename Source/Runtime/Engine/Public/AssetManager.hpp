#pragma once
#include "Object.hpp"

namespace oeng::engine
{
class ENGINE_API AssetManager
{
public:
    /**
     * Returns the asset if it is already loaded, otherwise load it.
     * @tparam T Asset type. Must match actual type.
     * @param path Asset path.
     * @return Loaded asset.
     * @throw std::bad_cast If the type `T` does not match the type of already loaded asset.
     * @throw std::exception Any exceptions thrown from the constructor of type `T`.
     */
    template <std::derived_from<OObject> T>
    [[nodiscard]] SharedRef<T> Load(Path path)
    {
        if (const auto found = assets_.find(path); found != assets_.end())
            return DynamicCast<T>(SharedRef{found->second});

        SharedRef loaded{new T{path}, [this, path](T* p)
        {
            EXPECT(assets_.erase(path) == 1);
            delete p;
        }};
        assets_.emplace(path, loaded);
        return loaded;
    }

    template <std::constructible_from<>>

private:
    std::unordered_map<Path, WeakPtr<OObject>> assets_;
};
}
