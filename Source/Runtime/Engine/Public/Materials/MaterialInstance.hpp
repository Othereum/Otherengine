#pragma once
#include "ShaderParam.hpp"
#include "Materials/MaterialInterface.hpp"

namespace oeng
{
inline namespace rhi
{
class RHIResource;
}

inline namespace engine
{
class Material;

class ENGINE_API MaterialInstance : public MaterialInterface, public ISerializable
{
public:
    void Serialize(Archive& ar) override;

    /**
     *
     */
    void SetParam(Name name, const ShaderParam& value);

    // For internal use only
    void ApplyParams();

private:
    std::shared_ptr<Material> material_;
    std::shared_ptr<RHIResource> texture_;
    std::unordered_map<Name, ShaderParam> params_;
};
}
}
