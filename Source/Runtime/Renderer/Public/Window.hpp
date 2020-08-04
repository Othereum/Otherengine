#pragma once
#include "Core.hpp"
#include "Math.hpp"

struct SDL_Window;

RENDERER_BEGIN

class RENDERER_API Window
{
public:
	DELETE_CPMV(Window);
	
	Window();
	~Window();
	
	[[nodiscard]] Vec2u16 GetSize() const noexcept;

	void SetTitle(std::u8string_view title) const noexcept;
	
	void SwapBuffer() const noexcept;

private:
	SDL_Window* window_;
	void* gl_context_;
};

RENDERER_END
