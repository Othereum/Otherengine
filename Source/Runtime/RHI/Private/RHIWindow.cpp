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
