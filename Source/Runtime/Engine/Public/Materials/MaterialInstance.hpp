#pragma once
#include "Materials/MaterialInterface.hpp"

namespace oeng
{
inline namespace engine
{
class Material;

class ENGINE_API MaterialInstance final : public MaterialInterface
{
CLASS_BODY(MaterialInstance)

public:
    void from_json(const Json& json) override;
    RHIShader& GetRHI() const noexcept override;

    [[nodiscard]] Material& GetParent() const noexcept
    {
        return *parent_;
    }

protected:
    [[nodiscard]] bool IsScalarParam(Name name) const override;
    [[nodiscard]] bool IsVectorParam(Name name) const override;
    [[nodiscard]] bool IsTextureParam(Name name) const override;

private:
    std::shared_ptr<Material> parent_;
};
}
}
