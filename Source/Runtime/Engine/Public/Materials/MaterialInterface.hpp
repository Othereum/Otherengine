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
    void LoadParams(const Json& json);

    std::unordered_map<Name, Float> scalars_;
    std::unordered_map<Name, Vec4> vectors_;
    std::unordered_map<Name, std::shared_ptr<Texture>> textures_;
};
}
}
