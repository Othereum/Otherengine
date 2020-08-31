#pragma once

namespace oeng
{
inline namespace core
{
class CORE_API Object : public std::enable_shared_from_this<Object>
{
INTERFACE_BODY(Object)

public:
    virtual void Serialize(class Archive& ar) = 0;
    [[nodiscard]] virtual Name GetClassName() const noexcept = 0;
};

inline Archive& operator<<(Archive& ar, Object& obj)
{
    obj.Serialize(ar);
    return ar;
}
}
}
