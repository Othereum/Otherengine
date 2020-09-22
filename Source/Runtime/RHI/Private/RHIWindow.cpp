#include "RHIWindow.hpp"
#include <SDL2/SDL_video.h>

namespace oeng
{
inline namespace rhi
{
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

Vec2i RHIWindow::GetSize() const noexcept
{
    Vec2i size;
    SDL_GetWindowSize(sdl_window.get(), &size[0], &size[1]);
    return size;
}
} // namespace rhi
} // namespace oeng
