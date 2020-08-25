#pragma once
#include "MaterialInterface.hpp"
#include "ShaderParam.hpp"

namespace oeng
{
inline namespace rhi
{
class RHIShader;
}

inline namespace engine
{
class ENGINE_API Material : public MaterialInterface, public ISerializable
{
public:
    void Serialize(Archive& ar) override;

private:
    std::shared_ptr<RHIShader> shader_;
    std::unordered_map<Name, ShaderParam> defaults_;
};
}
}
