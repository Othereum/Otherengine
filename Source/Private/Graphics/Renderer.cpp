#include "Graphics/Renderer.hpp"

#include <stdexcept>
#include <SDL.h>
#include <GL/glew.h>

#include "Components/SpriteComponent.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

#include "Actor.hpp"
#include "GameModule.hpp"

namespace oeng
{
	static void SetGlAttribute(SDL_GLattr attr, int value)
	{
		if (0 != SDL_GL_SetAttribute(attr, value))
			throw std::runtime_error{SDL_GetError()};
	}
	
	static WindowPtr CreateWindow(Vec2u16 scr)
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

		WindowPtr window{
			SDL_CreateWindow(GetGameName(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr[0], scr[1], SDL_WINDOW_OPENGL),
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
	
	static GlContextPtr CreateGlContext(SDL_Window& window)
	{
		GlContextPtr context{
			SDL_GL_CreateContext(&window),
			&SDL_GL_DeleteContext
		};
		if (!context) throw std::runtime_error{SDL_GetError()};
		
		InitGl();
		return context;
	}

	static std::unique_ptr<Shader> CreateSpriteShader(Vec2u16 scr)
	{
		auto shader = std::make_unique<Shader>("../Engine/Shaders/Sprite.vert", "../Engine/Shaders/Sprite.frag");
		shader->SetMatrixUniform("uViewProj", MakeSimpleViewProj<4>({scr}));
		return shader;
	}

	static std::unique_ptr<VertexArray> CreateSpriteVerts()
	{
		constexpr Vertex vertex_buffer[]
		{
			{{-0.5, 0.5, 0}, {}, {0, 0}},
			{{0.5, 0.5, 0}, {}, {1, 0}},
			{{0.5, -0.5, 0}, {}, {1, 1}},
			{{-0.5, -0.5, 0}, {}, {0, 1}}
		};

		constexpr Vec3u16 index_buffer[]
		{
			{0, 1, 2},
			{2, 3, 0}
		};

		return std::make_unique<VertexArray>(vertex_buffer, index_buffer);
	}

	Renderer::Renderer(Vec2u16 scr):
		window_{CreateWindow(scr)},
		gl_context_{CreateGlContext(*window_)},
		sprite_shader_{CreateSpriteShader(scr)},
		sprite_verts_{CreateSpriteVerts()}
	{
	}

	Renderer::~Renderer() = default;

	void Renderer::RegisterSprite(const CSpriteComponent& sprite)
	{
		auto cmp = [](const CSpriteComponent& a, const CSpriteComponent& b)
		{
			return a.GetDrawOrder() <= b.GetDrawOrder();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void Renderer::UnregisterSprite(const CSpriteComponent& sprite)
	{
		auto pr = [&](const CSpriteComponent& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void Renderer::DrawScene() const
	{
		glClearColor(.86f, .86f, .86f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		for (auto&& sprite_ref : sprites_)
		{
			auto&& sprite = sprite_ref.get();
			if (sprite.IsEnabled()) sprite.Draw(*sprite_shader_);
		}

		SDL_GL_SwapWindow(window_.get());
	}

	Vec2u16 Renderer::GetScreenSize() const noexcept
	{
		int w, h;
		SDL_GetWindowSize(window_.get(), &w, &h);
		return Vec2u16{Vector{w, h}};
	}
}
