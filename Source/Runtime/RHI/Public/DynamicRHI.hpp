#pragma once
#include <span>

namespace oeng
{
inline namespace rhi
{
class RHITexture;
class RHIMesh;
class RHIShader;
class RHIWindow;
struct Vertex;

class RHI_API DynamicRHI
{
  public:
    virtual ~DynamicRHI() = default;
    [[nodiscard]] static DynamicRHI& Get() noexcept;

    [[nodiscard]] virtual RHITexture* CreateTexture(Vec2i size, int channels, const unsigned char* pixels) const = 0;
    [[nodiscard]] virtual RHIMesh* CreateMesh(std::span<const Vertex> vertices,
                                              std::span<const Vec3u16> indices) const = 0;

    [[nodiscard]] virtual RHIShader* CreateShader(const char* vertex_shader, const char* frag_shader) const = 0;
    [[nodiscard]] virtual RHIWindow* CreateWindow(const char8_t* title, int x, int y, int w, int h, unsigned flags) = 0;

    virtual void PreDraw3D() const noexcept = 0;
    virtual void PreDraw2D() const noexcept = 0;
};
} // namespace rhi
} // namespace oeng
