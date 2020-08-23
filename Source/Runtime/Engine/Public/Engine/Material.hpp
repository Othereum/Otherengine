#pragma once
#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
class RHIResource;
class RHIShader;
}
}

namespace oeng
{
inline namespace engine
{
class ENGINE_API Material : public ISerializable
{
public:
    void Serialize(Archive& ar) override;
    void ApplyDefaultParams();

private:
    std::shared_ptr<RHIShader> shader_;
    std::shared_ptr<RHIResource> texture_;
    std::unordered_map<Name, ShaderParam> params_;
};
}
}
