#pragma once

namespace oeng
{
inline namespace engine
{
class Texture;

class MaterialInterface
{
public:
    INTERFACE_BODY(MaterialInterface);

    // For internal use only
    virtual void Activate() const = 0;

protected:
    std::unordered_map<Name, float> params_;
    std::unordered_map<Name, std::shared_ptr<Texture>> textures_;
};
}
}
