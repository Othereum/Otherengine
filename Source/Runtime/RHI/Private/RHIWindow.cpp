#include "RHIWindow.hpp"
#include <SDL2/SDL_video.h>

namespace logcat
{
const LogCategory kWindow{u8"Window"sv};
}

namespace oeng
{
inline namespace rhi
{
class WindowCreator
{
  public:
    WindowCreator()
    {
        WriteDisplayModes();
        LoadDisplayIdx();
        WriteWindowSize();
        LoadDisplayMode();
        SetupAttributes();
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
    void WriteDisplayModes() const
    {
        const auto num_dp = SDL_GetNumVideoDisplays();
        if (num_dp < 1)
            throw std::runtime_error{SDL_GetError()};

        auto& displays = cfg_["Display modes (read only)"] = Json::object();
        for (auto dp = 0; dp < num_dp; ++dp)
        {
            const auto num_dm = SDL_GetNumDisplayModes(dp);
            if (num_dm < 1)
                throw std::runtime_error{SDL_GetError()};

            auto& modes = displays[fmt::format("Display {}", dp)] = Json::array();
            for (auto dm = 0; dm < num_dm; ++dm)
            {
                SDL_DisplayMode mode;
                SDL_GetDisplayMode(dp, dm, &mode);
                auto str = fmt::format(u8"[{}] {}x{} {}Hz"sv, dm, mode.w, mode.h, mode.refresh_rate);
                modes.emplace_back(AsString(std::move(str)));
            }
        }
    }

    void WriteWindowSize() const
    {
        if (!cfg_.contains("WindowedSize"))
        {
            SDL_DisplayMode dp_mode;
            SDL_GetDesktopDisplayMode(dp_idx_, &dp_mode);
            Vector size{dp_mode.w, dp_mode.h};
            cfg_["WindowedSize"] = (size *= 5, size /= 6);
        }
    }

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

    static void SetupAttributes()
    {
    }

    const Name cfg_name_{u8"Display"sv};
    Json& cfg_{ConfigSystem::Get()(cfg_name_)};

    const bool fullscreen_ = cfg_.at("Fullscreen").get<bool>();
    const uint32_t flags_ = SDL_WINDOW_OPENGL | (fullscreen_ ? SDL_WINDOW_FULLSCREEN : 0);

    int dp_idx_ = cfg_.at("Display").get<int>();
    SDL_DisplayMode dp_mode_{};
};

static void LoadVSync()
{
    auto& cfg = ConfigSystem::Get()(u8"Display"sv);
    if (cfg.at("VSync").get<bool>())
    {
        if (cfg.at("VSync_Adaptive").get<bool>())
        {
            if (SDL_GL_SetSwapInterval(-1) == 0)
            {
                OE_LOG(kWindow, kInfo, u8"Adaptive sync enabled"sv);
            }
            else
            {
                OE_LOG(kWindow, kWarn, u8"Adaptive sync not supported. Falling back to normal VSync..."sv);
                SDL_GL_SetSwapInterval(1);
            }
        }
        else
        {
            OE_LOG(kWindow, kInfo, u8"VSync enabled"sv);
            SDL_GL_SetSwapInterval(1);
        }
    }
    else
    {
        OE_LOG(kWindow, kInfo, u8"VSync disabled"sv);
        SDL_GL_SetSwapInterval(0);
    }
}

void SDLWindowDeleter::operator()(SDL_Window* window) const noexcept
{
    SDL_DestroyWindow(window);
}

RHIWindow::RHIWindow(const char8_t* title, int x, int y, int w, int h, unsigned flags)
    : sdl_window{SDL_CreateWindow(AsString(title), x, y, w, h, flags)}
{
}

Vec2u16 RHIWindow::GetSize() const noexcept
{
    int w, h;
    SDL_GetWindowSize(sdl_window.get(), &w, &h);
    return Vec2u16(w, h);
}

void RHIWindow::SetTitle(const char8_t* title) const noexcept
{
    SDL_SetWindowTitle(sdl_window.get(), AsString(title));
}
} // namespace rhi
} // namespace oeng
