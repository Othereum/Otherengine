#pragma once

struct SDL_Window;

namespace logcat
{
extern ENGINE_API const LogCategory kWindow;
}

namespace oeng
{
inline namespace engine
{
class ENGINE_API Window
{
  public:
    void Initialize();
    void CleanUp();

    [[nodiscard]] Vec2u16 GetSize() const noexcept;
    void SetTitle(std::u8string_view title) const noexcept;
    void SwapBuffer() const noexcept;

  private:
    SDL_Window* window_ = nullptr;
    std::unique_ptr<void, void (*)(void*)> context_;
};
} // namespace engine
} // namespace oeng
