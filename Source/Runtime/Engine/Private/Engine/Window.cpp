#pragma once
#include "Engine/Window.hpp"
#include "DynamicRHI.hpp"
#include "RHIWindow.hpp"

namespace oeng
{
inline namespace engine
{
class WindowCreator
{
  public:
    WindowCreator()
    {
        LoadDisplayIdx();
        WriteWindowSize();
        LoadDisplayMode();
    }

    [[nodiscard]] SDL_Window* operator()() const
    {
        auto* const window =
            SDL_CreateWindow(AsString(EngineBase::Get().GetGameName().data()), SDL_WINDOWPOS_CENTERED_DISPLAY(dp_idx_),
                             SDL_WINDOWPOS_CENTERED_DISPLAY(dp_idx_), dp_mode_.w, dp_mode_.h, flags_);

        if (!window)
            throw std::runtime_error{SDL_GetError()};
        return window;
    }

    ~WindowCreator()
    {
        (void)ConfigSystem::Get().Save(cfg_name_);
    }

  private:
    void LoadDisplayIdx()
    {
        const auto num_dp = SDL_GetNumVideoDisplays();
        if (dp_idx_ >= num_dp)
        {
            OE_LOG(kWindow, kWarn, u8"Attempted to use a non-existent display (tried: {}, max: {})"sv, dp_idx_,
                   num_dp - 1);
            OE_LOG(kWindow, kWarn, u8"Using display 0..."sv);
            dp_idx_ = 0;
        }
    }

    void LoadDisplayMode()
    {
        if (fullscreen_)
            LoadFullscreenDpMode();
        else
            LoadWindowedDpMode();
    }

    void LoadFullscreenDpMode()
    {
        auto dm_idx = cfg_.at("FullscreenDisplayMode").get<int>();
        const auto num_dm = SDL_GetNumDisplayModes(dp_idx_);

        if (dm_idx >= num_dm)
        {
            OE_LOG(kWindow, kWarn, u8"Attempted to use a non-existent display mode (tried: {}, max: {})"sv, dm_idx,
                   num_dm - 1);
            OE_LOG(kWindow, kWarn, u8"Using display mode 0..."sv);
            dm_idx = 0;
        }

        if (0 != SDL_GetDisplayMode(dp_idx_, dm_idx, &dp_mode_))
            throw std::runtime_error{SDL_GetError()};

        OE_LOG(kWindow, kInfo, u8"Fullscreen mode: {}x{} {}Hz"sv, dp_mode_.w, dp_mode_.h, dp_mode_.refresh_rate);
    }

    void LoadWindowedDpMode()
    {
        const auto size = cfg_.at("WindowedSize").get<Vec2u16>();
        dp_mode_.w = size[0];
        dp_mode_.h = size[1];
        OE_LOG(kWindow, kInfo, u8"Windowed mode: {}x{}"sv, dp_mode_.w, dp_mode_.h);
    }

    const Name cfg_name_{u8"Display"sv};
    Json& cfg_{ConfigSystem::Get()(cfg_name_)};

    const bool fullscreen_ = cfg_.at("Fullscreen").get<bool>();
    const uint32_t flags_ = SDL_WINDOW_OPENGL | (fullscreen_ ? SDL_WINDOW_FULLSCREEN : 0);

    int dp_idx_ = cfg_.at("Display").get<int>();
    SDL_DisplayMode dp_mode_{};
};

Window::Window()
{
}

Window::Window(const char8_t* title, int x, int y, int w, int h, unsigned flags)
    : rhi_{DynamicRHI::Get().CreateWindow(title, x, y, w, h, flags)}
{
}

Window::~Window() = default;

void Window::SetTitle(const char8_t* title) const noexcept
{
    rhi_->SetTitle(title);
}

Vec2u16 Window::GetSize() const noexcept
{
    return rhi_->GetSize();
}
} // namespace engine
} // namespace oeng
