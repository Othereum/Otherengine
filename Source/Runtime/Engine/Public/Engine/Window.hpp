#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIWindow;
}

inline namespace engine
{
class ENGINE_API Window
{
  public:
    Window(const char8_t* title, int x, int y, int w, int h, unsigned flags);
    ~Window();

    void SetTitle(const char8_t* title) const noexcept;
    void SetSize(int w, int h) const;
    void SetRefreshRate(int hz) const;

    [[nodiscard]] bool IsFullscreen() const noexcept;
    [[nodiscard]] Vec2u16 GetSize() const noexcept;

    [[nodiscard]] RHIWindow& GetRHI() const noexcept
    {
        return *rhi_;
    }

  private:
    std::unique_ptr<RHIWindow> rhi_;
};
} // namespace engine
} // namespace oeng
