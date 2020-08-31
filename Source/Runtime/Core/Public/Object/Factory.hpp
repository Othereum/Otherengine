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
     * Creates an object with a class name and casts it to the target type.
     * @tparam T Target type.
     * @param name Class name.
     * @return Created object.
     * @throw std::out_of_range If the class name is not valid.
     * @throw std::bad_cast If the cast fails.
     */
    template <std::derived_from<Object> T>
    [[nodiscard]] std::shared_ptr<T> Create(Name name) const
    {
        auto ptr = Create(name);
        return {std::move(ptr), &dynamic_cast<T&>(*ptr)};
    }

    /**
     * Creates an object with a class name.
     * @param name Class name.
     * @return Created object.
     * @throw std::out_of_range If the class name is not valid.
     */
    [[nodiscard]] std::shared_ptr<Object> Create(Name name) const
    {
        return makers_.at(name)();
    }

    template <std::derived_from<Object> T>
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
