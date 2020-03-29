#include "Renderer.h"
#include <stdexcept>
#include <SDL.h>
#include <GL/glew.h>
#include <fmt/core.h>
#include "Component/SpriteComponent.h"

namespace game
{
	static void SetGlAttribute(SDL_GLattr attr, int value)
	{
		if (0 != SDL_GL_SetAttribute(attr, value))
			throw std::runtime_error{SDL_GetError()};
	}
	
	static CRenderer::TWindowPtr CreateWindow()
	{
		SetGlAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SetGlAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SetGlAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SetGlAttribute(SDL_GL_RED_SIZE, 8);
		SetGlAttribute(SDL_GL_GREEN_SIZE, 8);
		SetGlAttribute(SDL_GL_BLUE_SIZE, 8);
		SetGlAttribute(SDL_GL_ALPHA_SIZE, 8);
		SetGlAttribute(SDL_GL_DOUBLEBUFFER, true);
		SetGlAttribute(SDL_GL_ACCELERATED_VISUAL, true);
		
		const auto window = SDL_CreateWindow(PROJECT_NAME, 100, 100, kScrSz.x, kScrSz.y, SDL_WINDOW_OPENGL);
		if (!window) throw std::runtime_error{SDL_GetError()};
		return {window, &SDL_DestroyWindow};
	}

	static CRenderer::GlContextPtr CreateGlContext(SDL_Window& window)
	{
		const auto context = SDL_GL_CreateContext(&window);
		if (!context) throw std::runtime_error{SDL_GetError()};
		return {context, &SDL_GL_DeleteContext};
	}
	
	CRenderer::CRenderer()
		:window_{CreateWindow()}, gl_context_{CreateGlContext(*window_)}
	{
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			throw std::runtime_error{fmt::format("Failed to initialize GLEW. Error code: {}", glGetError())};
		}
	}

	void CRenderer::RegisterSprite(const CSpriteComponent& sprite)
	{
		auto cmp = [](const CSpriteComponent& a, const CSpriteComponent& b)
		{
			return a.GetDrawOrder() <= b.GetDrawOrder();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void CRenderer::UnregisterSprite(const CSpriteComponent& sprite)
	{
		auto pr = [&](const CSpriteComponent& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void CRenderer::Draw(SDL_Texture& texture, const TFRect& dest, Degrees angle) const
	{
		const SDL_Rect r = dest;
		SDL_RenderCopyEx(renderer_.get(), &texture, nullptr, &r, angle.Get(), nullptr, SDL_FLIP_NONE);
	}

	void CRenderer::Draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, Degrees angle) const
	{
		SDL_RenderCopyEx(renderer_.get(), &texture, &src, &dest, angle.Get(), nullptr, SDL_FLIP_NONE);
	}

	void CRenderer::DrawScene() const
	{
		glClearColor(.86f, .86f, .86f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto& sprite : sprites_)
		{
			sprite.get().DrawSprite();
		}

		SDL_GL_SwapWindow(window_.get());
	}
}
