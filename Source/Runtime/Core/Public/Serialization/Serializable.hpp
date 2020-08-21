#pragma once

namespace oeng::core
{
class ISerializable
{
public:
    INTERFACE_BODY(ISerializable);
    virtual void Serialize(class Archive& ar) = 0;
};
}
