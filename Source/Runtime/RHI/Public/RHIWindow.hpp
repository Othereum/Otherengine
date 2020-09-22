#pragma once

struct SDL_Window;

namespace oeng
{
inline namespace rhi
{
enum class SwapInterval
{
    adaptive = -1,
    immediate,
    synchronized
};

class RHI_API SDLWindowDeleter
{
  public:
    void operator()(SDL_Window* window) const noexcept;
};

class RHI_API RHIWindow
{
  public:
    RHIWindow(const char8_t* title, int x, int y, int w, int h, unsigned flags);
    virtual ~RHIWindow() = default;

    virtual void MakeCurrent() const noexcept = 0;
    virtual void SwapBuffer() const noexcept = 0;
    virtual bool SetSwapInterval(SwapInterval interval) = 0;

    void SetTitle(const char8_t* title) const noexcept;
    void SetSize(int w, int h) const;
    void SetRefreshRate(int hz) const;
    [[nodiscard]] bool IsFullscreen() const noexcept;
    [[nodiscard]] Vec2u16 GetSize() const noexcept;

  protected:
    const std::unique_ptr<SDL_Window, SDLWindowDeleter> sdl_window;
};
} // namespace rhi
} // namespace oeng
