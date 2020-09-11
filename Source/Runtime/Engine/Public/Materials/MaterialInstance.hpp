#pragma once
#include "Materials/IMaterial.hpp"

namespace oeng
{
inline namespace engine
{
class Material;

class ENGINE_API MaterialInstance final : public IMaterial
{
    CLASS_BODY(MaterialInstance)

  public:
    MaterialInstance();

    void from_json(const Json& json) override;
    RHIShader& GetRHI() const noexcept override;

    [[nodiscard]] auto& GetRoot() const noexcept
    {
        return root_;
    }

    [[nodiscard]] auto& GetParent() const noexcept
    {
        return parent_;
    }

  protected:
    [[nodiscard]] bool IsScalarParam(Name name) const override;
    [[nodiscard]] bool IsVectorParam(Name name) const override;
    [[nodiscard]] bool IsTextureParam(Name name) const override;

  private:
    SharedRef<Material> root_;
    SharedRef<IMaterial> parent_;
};
} // namespace engine
} // namespace oeng
