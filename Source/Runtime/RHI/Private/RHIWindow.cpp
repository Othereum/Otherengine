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

void RHIWindow::SetTitle(const char8_t* title) const noexcept
{
    SDL_SetWindowTitle(sdl_window.get(), AsString(title));
}

void RHIWindow::SetSize(int w, int h) const
{
    if (IsFullscreen())
    {
        SDL_DisplayMode dm;

        if (0 != SDL_GetWindowDisplayMode(sdl_window.get(), &dm))
            throw std::runtime_error{SDL_GetError()};

        dm.w = w, dm.h = h;

        if (0 != SDL_SetWindowDisplayMode(sdl_window.get(), &dm))
            throw std::runtime_error{SDL_GetError()};
    }
    else
    {
        SDL_SetWindowSize(sdl_window.get(), w, h);
    }
}

void RHIWindow::SetRefreshRate(int hz) const
{
    if (!IsFullscreen())
        return;

    SDL_DisplayMode dm;

    if (0 != SDL_GetWindowDisplayMode(sdl_window.get(), &dm))
        throw std::runtime_error{SDL_GetError()};

    dm.refresh_rate = hz;

    if (0 != SDL_SetWindowDisplayMode(sdl_window.get(), &dm))
        throw std::runtime_error{SDL_GetError()};
}

bool RHIWindow::IsFullscreen() const noexcept
{
    return SDL_WINDOW_FULLSCREEN & SDL_GetWindowFlags(sdl_window.get());
}

Vec2u16 RHIWindow::GetSize() const noexcept
{
    int w, h;
    SDL_GetWindowSize(sdl_window.get(), &w, &h);
    return Vec2u16(w, h);
}
} // namespace rhi
} // namespace oeng
