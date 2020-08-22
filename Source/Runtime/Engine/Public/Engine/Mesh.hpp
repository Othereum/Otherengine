#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIResource;
}
}

namespace oeng
{
inline namespace engine
{
class ENGINE_API Mesh : public ISerializable
{
public:
    void Serialize(Archive& ar) override;

    [[nodiscard]] Float Radius() const noexcept
    {
        return radius_;
    }

private:
    std::shared_ptr<class Material> material_;
    std::unique_ptr<RHIResource> rhi_;
    Float radius_ = 0;
};
}
}
