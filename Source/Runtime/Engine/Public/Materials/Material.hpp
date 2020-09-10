#pragma once
#include "IMaterial.hpp"

namespace oeng
{
inline namespace rhi
{
class RHIShader;
}

inline namespace engine
{
class ENGINE_API Material final : public IMaterial
{
    CLASS_BODY(Material)

  public:
    [[nodiscard]] static SharedRef<Material> GetDefault();

    Material();
    ~Material();

    void from_json(const Json& json) override;
    RHIShader& GetRHI() const noexcept override;

  protected:
    bool IsScalarParam(Name name) const override;
    bool IsVectorParam(Name name) const override;
    bool IsTextureParam(Name name) const override;

  private:
    std::unique_ptr<RHIShader> shader_;
};
} // namespace engine
} // namespace oeng
