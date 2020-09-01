#pragma once

namespace oeng
{
inline namespace core
{
class CORE_API Object : public std::enable_shared_from_this<Object>
{
INTERFACE_BODY(Object)

public:
    [[nodiscard]] virtual Name GetClassName() const noexcept = 0;

    virtual void from_json([[maybe_unused]] const Json& json)
    {
    }
};
}
}
