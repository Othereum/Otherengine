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

[[nodiscard]] CORE_API std::shared_ptr<Object> NewObject(Name type);

template <class T>
[[nodiscard]] std::shared_ptr<T> NewObject(Name type)
{
    auto ptr = NewObject(type);
    return {std::move(ptr), *dynamic_cast<T&>(*ptr)};
}

CORE_API void RegisterClass(Name type, std::shared_ptr<Object> (*creator)());
}
}
