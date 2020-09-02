#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIMesh;
}
}

namespace oeng
{
inline namespace engine
{
class IMaterial;

class ENGINE_API Mesh final : public Object
{
CLASS_BODY(Mesh)

public:
    void from_json(const Json& json) override;

    [[nodiscard]] Float Radius() const noexcept
    {
        return radius_;
    }

    [[nodiscard]] RHIMesh& GetRHI() const noexcept
    {
        return *rhi_;
    }

private:
    std::shared_ptr<IMaterial> material_;
    std::unique_ptr<RHIMesh> rhi_;
    Float radius_ = 0;
};
}
}
