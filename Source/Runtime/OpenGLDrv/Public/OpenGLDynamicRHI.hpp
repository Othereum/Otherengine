#pragma once
#include "DynamicRHI.hpp"

struct SDL_Window;

namespace logcat
{
extern OPENGLDRV_API const LogCategory kOpenGL;
}

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLContextDeleter
{
  public:
    void operator()(void* context) const noexcept;
};

class OPENGLDRV_API OpenGLDynamicRHI final : public DynamicRHI
{
  public:
    OpenGLDynamicRHI();

    [[nodiscard]] RHITexture* CreateTexture(Vec2i size, int channels, const unsigned char* pixels) const override;
    [[nodiscard]] RHIMesh* CreateMesh(std::span<const Vertex> vertices,
                                      std::span<const Vec3u16> indices) const override;

    [[nodiscard]] RHIShader* CreateShader(const char* vertex_shader, const char* frag_shader) const override;
    [[nodiscard]] RHIWindow* CreateWindow(const char8_t* title, int x, int y, int w, int h, unsigned flags) override;

    void PreDraw3D() const noexcept override;
    void PreDraw2D() const noexcept override;

    [[nodiscard]] void* GetContext(SDL_Window* window);

  private:
    std::unique_ptr<void, OpenGLContextDeleter> context_;
};
} // namespace opengldrv
} // namespace oeng
