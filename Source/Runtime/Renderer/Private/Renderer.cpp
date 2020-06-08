#include "Graphics/Renderer.hpp"

#include <stdexcept>
#include <SDL.h>
#include <GL/glew.h>

#include "Components/SpriteComponent.hpp"
#include "Components/MeshComponent.hpp"
#include "Graphics/VertexArray.hpp"
#include "Graphics/Shader.hpp"
#include "Actor.hpp"
#include "Engine.hpp"

namespace oeng
{
	static void SetGlAttribute(SDL_GLattr attr, int value)
	{
		if (0 != SDL_GL_SetAttribute(attr, value))
			throw std::runtime_error{SDL_GetError()};
	}
	
	static WindowPtr CreateWindow(const char* title, Vec2u16 scr)
	{
		SetGlAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SetGlAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SetGlAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SetGlAttribute(SDL_GL_RED_SIZE, 8);
		SetGlAttribute(SDL_GL_GREEN_SIZE, 8);
		SetGlAttribute(SDL_GL_BLUE_SIZE, 8);
		SetGlAttribute(SDL_GL_ALPHA_SIZE, 8);
		SetGlAttribute(SDL_GL_DEPTH_SIZE, 24);
		SetGlAttribute(SDL_GL_DOUBLEBUFFER, true);
		SetGlAttribute(SDL_GL_ACCELERATED_VISUAL, true);

		WindowPtr window{
			SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scr[0], scr[1], SDL_WINDOW_OPENGL),
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

	static VertexArray CreateSpriteVerts()
	{
		constexpr Vertex vertex_buffer[]
		{
			{{-0.5_f, 0.5_f, 0_f}, {}, {0_f, 0_f}},
			{{0.5_f, 0.5_f, 0_f}, {}, {1_f, 0_f}},
			{{0.5_f, -0.5_f, 0_f}, {}, {1_f, 1_f}},
			{{-0.5_f, -0.5_f, 0_f}, {}, {0_f, 1_f}}
		};

		constexpr Vec3u16 index_buffer[]
		{
			{0, 1, 2},
			{2, 3, 0}
		};

		return {vertex_buffer, index_buffer};
	}

	Renderer::Renderer(Engine& engine, Vec2u16 scr)
		:engine_{engine}, scr_sz_{scr},
		window_{CreateWindow(engine.GetGameName().data(), scr)},
		gl_context_{CreateGlContext(*window_)},
		basic_mesh_shader_{"../Engine/Shaders/BasicMesh.vert", "../Engine/Shaders/BasicMesh.frag"},
		sprite_shader_{"../Engine/Shaders/Sprite.vert", "../Engine/Shaders/Sprite.frag"},
		sprite_verts_{CreateSpriteVerts()}
	{
		sprite_shader_.SetViewProj(MakeSimpleViewProj<4>(scr));
	}

	Renderer::~Renderer() = default;

	void Renderer::RegisterSprite(const SpriteComponent& sprite)
	{
		auto cmp = [](const SpriteComponent& a, const SpriteComponent& b)
		{
			return a.GetDrawOrder() <= b.GetDrawOrder();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void Renderer::UnregisterSprite(const SpriteComponent& sprite)
	{
		auto pr = [&](const SpriteComponent& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void Renderer::RegisterMesh(const MeshComponent& mesh)
	{
		meshes_.emplace_back(mesh);
	}

	void Renderer::UnregisterMesh(const MeshComponent& mesh)
	{
		auto pr = [&](const MeshComponent& v) { return &v == &mesh; };
		const auto found = std::find_if(meshes_.crbegin(), meshes_.crend(), pr);
		if (found != meshes_.crend()) meshes_.erase(found.base() - 1);
	}

	void Renderer::DrawScene()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		basic_mesh_shader_.Activate();
		const auto view = MakeLookAt(Vec3::zero, Vec3::forward, Vec3::up);
		const auto proj = MakePerspective(Vec2{GetScreenSize()}, 25_f, 10000_f, 70_deg);
		basic_mesh_shader_.SetViewProj(view * proj);
		for (auto mesh : meshes_)
		{
			auto&& m = mesh.get();
			if (m.IsEnabled()) m.Draw(basic_mesh_shader_);
		}

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		sprite_shader_.Activate();
		for (auto sprite : sprites_)
		{
			auto&& s = sprite.get();
			if (s.IsEnabled()) s.Draw(sprite_shader_);
		}

		SDL_GL_SwapWindow(window_.get());
	}
}
