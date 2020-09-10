#pragma once

namespace oeng
{
inline namespace rhi
{
class RHITexture;
}

inline namespace engine
{
class ENGINE_API Texture final : public Object
{
    CLASS_BODY(Texture)

  public:
    Texture();
    ~Texture();

    [[nodiscard]] static SharedRef<Texture> GetDefault();

    void from_json(const Json& json) override;

    [[nodiscard]] Vec2i Size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] RHITexture& GetRHI() const noexcept
    {
        return *rhi_;
    }

  private:
    Vec2i size_;
    std::unique_ptr<RHITexture> rhi_;
};
} // namespace engine
} // namespace oeng
