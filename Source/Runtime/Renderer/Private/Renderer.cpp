#include "Renderer.hpp"

#include <stdexcept>
#include <SDL.h>
#include <GL/glew.h>

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Interfaces/Engine.hpp"
#include "Interfaces/Drawable.hpp"

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

	DefaultCamera::DefaultCamera(Vec2u16 scr) noexcept
		:scr_{scr}
	{
		RecalcViewProj();
	}

	Mat4 DefaultCamera::GetViewProj(Vec2u16 scr) const noexcept
	{
		if (scr != scr_) [[unlikely]] RecalcViewProj();
		return view_proj_;
	}

	void DefaultCamera::RecalcViewProj() const noexcept
	{
		static const auto view = MakeLookAt(Vec3::zero, Vec3::forward, Vec3::up);
		proj_ = MakePerspective(Vec2{scr_}, 25_f, 10000_f, 70_deg);
		view_proj_ = view * proj_;
	}

	Renderer::Renderer(IEngine& engine, Vec2u16 scr)
		:engine_{engine}, scr_sz_{scr},
		window_{CreateWindow(engine.GetGameName().data(), scr)},
		gl_context_{CreateGlContext(*window_)},
		sprite_shader_{"../Engine/Shaders/Sprite"},
		sprite_verts_{CreateSpriteVerts()},
		camera_{&default_camera_},
		default_camera_{scr}
	{
		sprite_shader_.SetViewProj(MakeSimpleViewProj<4>(scr));
	}

	Renderer::~Renderer() = default;

	void Renderer::RegisterSprite(const ISprite& sprite)
	{
		auto cmp = [](const ISprite& a, const ISprite& b)
		{
			return a.GetDrawOrder() <= b.GetDrawOrder();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void Renderer::UnregisterSprite(const ISprite& sprite)
	{
		auto pr = [&](const ISprite& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void Renderer::RegisterMesh(const IMesh& mesh)
	{
		auto shader_path = mesh.GetShaderPath();
		shaders_.try_emplace(shader_path, shader_path);
		mesh_comps_[shader_path].emplace_back(mesh);
	}

	void Renderer::UnregisterMesh(const IMesh& mesh)
	{
		const auto shader = mesh.GetShaderPath();
		const auto vec_it = mesh_comps_.find(shader);
		auto& vec = vec_it->second;
		
		auto pr = [&](const IMesh& v) { return &v == &mesh; };
		const auto found = std::find_if(vec.crbegin(), vec.crend(), pr);
		if (found != vec.crend()) vec.erase(found.base() - 1);
		
		if (vec.empty())
		{
			shaders_.erase(shader);
			mesh_comps_.erase(vec_it);
		}
	}

	void Renderer::UnregisterCamera() noexcept
	{
		camera_ = &default_camera_;
	}

	void Renderer::DrawScene()
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		const auto view_proj = camera_->GetViewProj(GetScreenSize());

		for (auto& pair : mesh_comps_)
		{
			auto& shader = shaders_.at(pair.first);
			shader.SetViewProj(view_proj);
			
			for (auto mesh : pair.second)
			{
				if (auto info = mesh.get().Draw())
				{
					shader.SetTransform(info->transform);
					glDrawElements(GL_TRIANGLES, info->vertices, GL_UNSIGNED_SHORT, nullptr);
				}
			}
		}

		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		sprite_shader_.Activate();
		for (auto sprite : sprites_)
		{
			if (auto info = sprite.get().Draw())
			{
				sprite_shader_.SetTransform(info->transform);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
			}
		}

		SDL_GL_SwapWindow(window_.get());
	}

	SharedPtr<Texture> Renderer::GetTexture(Path file)
	{
		const auto found = textures_.find(file);
		if (found != textures_.end()) return found->second.lock();

		SharedPtr<Texture> loaded{
			New<Texture>(file),
			[this, file](Texture* p) noexcept
			{
				textures_.erase(file);
				Delete(p);
			}
		};

		textures_.emplace(file, loaded);
		return loaded;
	}

	SharedPtr<Mesh> Renderer::GetMesh(Path file)
	{
		const auto found = meshes_.find(file);
		if (found != meshes_.end()) return found->second.lock();

		SharedPtr<Mesh> loaded{
			New<Mesh>(file, *this),
			[this, file](Mesh* p) noexcept
			{
				meshes_.erase(file);
				Delete(p);
			}
		};

		meshes_.emplace(file, loaded);
		return loaded;
	}
}
