#pragma once
#include "Path.hpp"

namespace oeng
{
inline namespace core
{
class CORE_API Object : public std::enable_shared_from_this<Object>
{
INTERFACE_BODY(Object)

public:
    [[nodiscard]] virtual Name GetClassName() const noexcept = 0;

    void from_json(const Json& json, Path path)
    {
        path_ = path;
        from_json(json);
    }

    [[nodiscard]] Path GetPath() const noexcept
    {
        return path_;
    }

protected:
    virtual void from_json([[maybe_unused]] const Json& json)
    {
    }

private:
    Path path_;
};

/**
 * Casts object pointer dynamically.
 * @throw std::bad_cast If the cast fails.
 */
template <class To, class From>
[[nodiscard]] std::shared_ptr<To> Cast(std::shared_ptr<From> ptr)
{
    return {std::move(ptr), *dynamic_cast<To&>(*ptr)};
}

/**
 * Creates an object with a class name.
 * @tparam T Target type.
 * @param type Class name.
 * @return Created object.
 * @throw std::out_of_range If the type name is not valid.
 */
[[nodiscard]] CORE_API std::shared_ptr<Object> NewObject(Name type);

/**
 * Creates an object with a class name and casts it to the target type.
 * @tparam T Target type.
 * @param type Class name.
 * @return Created object.
 * @throw std::out_of_range If the type name is not valid.
 * @throw std::bad_cast If the cast fails.
 */
template <class T>
[[nodiscard]] std::shared_ptr<T> NewObject(Name type)
{
    return Cast<T>(NewObject(type));
}

CORE_API void RegisterClass(Name type, std::shared_ptr<Object> (*creator)());
}
}
