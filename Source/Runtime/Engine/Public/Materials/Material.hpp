#pragma once
#include "MaterialInterface.hpp"
#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
class RHIResource;
class RHIShader;
}

inline namespace engine
{
class ENGINE_API Material : public MaterialInterface
{
public:

private:
    std::shared_ptr<RHIShader> shader_;
    std::shared_ptr<RHIResource> texture_;
    std::unordered_map<Name, ShaderParam> params_;
};
}
}
