#pragma once
#include "Engine/Window.hpp"
#include "DynamicRHI.hpp"
#include "RHIWindow.hpp"

namespace oeng
{
inline namespace engine
{
Window::Window(const char8_t* title, int x, int y, int w, int h, unsigned flags)
    : rhi_{DynamicRHI::Get().CreateWindow(title, x, y, w, h, flags)}
{
}

Window::~Window() = default;

void Window::SetTitle(const char8_t* title) const noexcept
{
    rhi_->SetTitle(title);
}

void Window::SetSize(int w, int h) const
{
    rhi_->SetSize(w, h);
}

void Window::SetRefreshRate(int hz) const
{
    rhi_->SetRefreshRate(hz);
}

bool Window::IsFullscreen() const noexcept 
{
    return rhi_->IsFullscreen();
}

Vec2u16 Window::GetSize() const noexcept
{
    return rhi_->GetSize();
}
} // namespace engine
} // namespace oeng
