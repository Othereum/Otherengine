#pragma once
#include "RHIWindow.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OpenGLDynamicRHI;

class OPENGLDRV_API OpenGLWindow : public RHIWindow
{
  public:
    OpenGLWindow(OpenGLDynamicRHI& rhi, const char8_t* title, int x, int y, int w, int h, unsigned flags);
    void MakeCurrent() const noexcept override;
    void SwapBuffer() const noexcept override;
    bool SetSwapInterval(SwapInterval interval) override;

  private:
    void* const context_;
};
} // namespace opengldrv
} // namespace oeng
