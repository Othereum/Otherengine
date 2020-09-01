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
class MaterialInterface;

class ENGINE_API Mesh final : public Object
{
CLASS_BODY(Mesh)

public:
    void Activate() const noexcept;

    [[nodiscard]] Float Radius() const noexcept
    {
        return radius_;
    }

    void from_json(const Json& json) override;

private:
    std::shared_ptr<MaterialInterface> material_;
    std::unique_ptr<RHIMesh> rhi_;
    Float radius_ = 0;
};
}
}
