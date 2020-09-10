#pragma once

struct SDL_Window;

namespace logcat
{
extern RENDERER_API const LogCategory kWindow;
}

namespace oeng
{
inline namespace renderer
{
class RENDERER_API Window
{
  public:
    Window();
    ~Window();

    [[nodiscard]] Vec2u16 GetSize() const noexcept;
    void SetTitle(std::u8string_view title) const noexcept;
    void SwapBuffer() const noexcept;

  private:
    SDL_Window* window_;
    void* gl_context_;
};
} // namespace renderer
} // namespace oeng
