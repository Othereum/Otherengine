#include "OpenGLWindow.hpp"
#include "OpenGLDynamicRHI.hpp"
#include <SDL2/SDL_video.h>

namespace oeng
{
inline namespace opengldrv
{
OpenGLWindow::OpenGLWindow(OpenGLDynamicRHI& rhi, const char8_t* title, int x, int y, int w, int h, unsigned flags)
    : RHIWindow{title, x, y, w, h, flags | SDL_WINDOW_OPENGL}, context_{rhi.GetContext(sdl_window.get())}
{
}

void OpenGLWindow::MakeCurrent() const noexcept
{
    SDL_GL_MakeCurrent(sdl_window.get(), context_);
}

void OpenGLWindow::SwapBuffer() const noexcept
{
    SDL_GL_SwapWindow(sdl_window.get());
}

bool OpenGLWindow::SetSwapInterval(SwapInterval interval)
{
    auto cur_context = SDL_GL_GetCurrentContext();
    auto cur_window = SDL_GL_GetCurrentWindow();

    if (ENSURE(cur_context == context_ && cur_window == sdl_window.get()))
        return SDL_GL_SetSwapInterval(static_cast<int>(interval)) == 0;

    return false;
}
} // namespace opengldrv
} // namespace oeng
