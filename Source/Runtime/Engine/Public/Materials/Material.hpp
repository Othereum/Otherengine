#pragma once
#include "MaterialInterface.hpp"

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
    std::unique_ptr<RHIShader> shader_;
};
}
}
