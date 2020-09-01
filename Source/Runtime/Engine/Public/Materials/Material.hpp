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
class ENGINE_API Material final : public MaterialInterface
{
CLASS_BODY(Material)

public:
    void from_json(const Json& json) override;
    RHIShader& GetRHI() const noexcept override;

protected:
    bool IsScalarParam(Name name) const override;
    bool IsVectorParam(Name name) const override;
    bool IsTextureParam(Name name) const override;

private:
    std::unique_ptr<RHIShader> shader_;
};
}
}
