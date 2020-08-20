#pragma once

namespace oeng
{
inline namespace rhi
{
class RHI_API Texture
{
public:
    DELETE_CPMV(Texture);

    /**
     * Load texture from image file (png, jpg, ...)
     * @param filepath Image file path.
     * @throw std::runtime_error If failed to load texture
     */
    explicit Texture(const fs::path& filepath);

    virtual ~Texture() = default;

    virtual void Activate() const noexcept = 0;

    [[nodiscard]] Vec2i size() const noexcept
    {
        return size_;
    }

    [[nodiscard]] int channels() const noexcept
    {
        return channels_;
    }

    [[nodiscard]] const unsigned char* data() const noexcept
    {
        return data_.get();
    }

private:
    Vec2i size_;
    int channels_ = 0;
    std::unique_ptr<const unsigned char[]> data_;
};
}
}
