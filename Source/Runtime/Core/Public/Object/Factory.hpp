#pragma once

namespace oeng
{
inline namespace core
{
class Object;

class CORE_API Factory
{
public:
    [[nodiscard]] static Factory& Get();

    template <std::derived_from<Object> T>
    [[nodiscard]] static std::shared_ptr<Object> Create()
    {
        return std::make_shared<T>();
    }

    /**
     * Creates an object with a type name and casts it to the target type.
     * @tparam T Target type.
     * @param type Type name.
     * @return Created object.
     * @throw std::out_of_range If the type name is not valid.
     * @throw std::bad_cast If the cast fails.
     */
    template <class T>
    [[nodiscard]] std::shared_ptr<T> Create(Name type) const
    {
        auto ptr = Create(type);
        return {std::move(ptr), &dynamic_cast<T&>(*ptr)};
    }

    /**
     * Creates an object with a type name.
     * @param type Type name.
     * @return Created object.
     * @throw std::out_of_range If the type name is not valid.
     */
    [[nodiscard]] std::shared_ptr<Object> Create(Name type) const
    {
        return makers_.at(type)();
    }

    template <class T>
    void Register()
    {
        std::shared_ptr<Object> (*fn)() = &Create<T>;
        const auto [it, inserted] = makers_.emplace(T::class_name, fn);
        assert((inserted || it->second == fn) && "A name conflict has occured.");
    }

private:
    std::unordered_map<Name, std::shared_ptr<Object>(*)()> makers_;
};
}
}
