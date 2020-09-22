#include "OpenGLWindow.hpp"
#include "OpenGLDynamicRHI.hpp"
#include <GL/glew.h>
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

void OpenGLWindow::ClearBuffer() const noexcept
{
    assert(IsCurrent());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool OpenGLWindow::SetSwapInterval(SwapInterval interval) const noexcept
{
    assert(IsCurrent());
    return 0 == SDL_GL_SetSwapInterval(static_cast<int>(interval));
}

bool OpenGLWindow::IsCurrent() const noexcept
{
    auto cur_context = SDL_GL_GetCurrentContext();
    auto cur_window = SDL_GL_GetCurrentWindow();
    return cur_context == context_ && cur_window == sdl_window.get();
}
} // namespace opengldrv
} // namespace oeng
