#include "Graphics/Renderer.h"

#include <stdexcept>
#include <SDL.h>
#include <GL/glew.h>

#include "Components/SpriteComponent.h"
#include "Graphics/VertexArray.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.hpp"

#include "Actor.h"
#include "GameModule.hpp"

namespace oeng::graphics
{
	static void SetGlAttribute(SDL_GLattr attr, int value)
	{
		if (0 != SDL_GL_SetAttribute(attr, value))
			throw std::runtime_error{SDL_GetError()};
	}
	
	static auto CreateWindow()
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

		CRenderer::TWindowPtr window
		{
			SDL_CreateWindow(GetGameName(), 100, 100, kScrSz.x, kScrSz.y, SDL_WINDOW_OPENGL),
			&SDL_DestroyWindow
		};
		if (!window) throw std::runtime_error{SDL_GetError()};
		return window;
	}

	static void InitGl()
	{
		glewExperimental = true;
		
		if (const auto err = glewInit(); err != GLEW_OK)
			throw std::runtime_error{reinterpret_cast<const char*>(glewGetErrorString(err))};
		
		// On some platforms, GLEW will emit a benign error code, so clear it
		glGetError();
	}
	
	static auto CreateGlContext(SDL_Window& window)
	{
		CRenderer::GlContextPtr context
		{
			SDL_GL_CreateContext(&window),
			&SDL_GL_DeleteContext
		};
		if (!context) throw std::runtime_error{SDL_GetError()};
		
		InitGl();
		return context;
	}

	static auto CreateSpriteShader()
	{
		auto shader = std::make_unique<Shader>("../Engine/Shaders/Sprite.vert", "../Engine/Shaders/Sprite.frag");
		shader->SetMatrixUniform("uViewProj", Mat4::SimpleViewProj(Vec2{kScrSz}));
		return shader;
	}

	static auto CreateSpriteVerts()
	{
		constexpr Vertex vertex_buffer[]
		{
			{{-0.5, 0.5, 0}, {}, {0, 0}},
			{{0.5, 0.5, 0}, {}, {1, 0}},
			{{0.5, -0.5, 0}, {}, {1, 1}},
			{{-0.5, -0.5, 0}, {}, {0, 1}}
		};

		constexpr Vector<uint16_t, 3> index_buffer[]
		{
			{0, 1, 2},
			{2, 3, 0}
		};

		return std::make_unique<VertexArray>(vertex_buffer, index_buffer);
	}

	CRenderer::CRenderer():
		window_{CreateWindow()},
		gl_context_{CreateGlContext(*window_)},
		sprite_shader_{CreateSpriteShader()},
		sprite_verts_{CreateSpriteVerts()}
	{
	}

	CRenderer::~CRenderer() = default;

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

	void CRenderer::Draw(const CSpriteComponent& sprite) const
	{
		if (sprite.IsEnabled())
		{
			static const Name name = "uWorldTransform";
			sprite_shader_->SetMatrixUniform(name, Mat4::Scale({sprite.GetTexture().Size(), 1}) * sprite.GetOwner().GetTransformMatrix());
			sprite.GetTexture().Activate();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
		}
	}

	void CRenderer::DrawScene() const
	{
		glClearColor(.86f, .86f, .86f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (const auto& sprite : sprites_)
		{
			Draw(sprite);
		}

		SDL_GL_SwapWindow(window_.get());
	}
}
