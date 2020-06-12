#include "Renderer.hpp"

#include <stdexcept>
#include <SDL.h>
#include "OpenGL.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include "Interfaces/Engine.hpp"
#include "Interfaces/Drawable.hpp"
#include "Interfaces/Light.hpp"

namespace oeng
{
	static void SetGlAttribute(SDL_GLattr attr, int value)
	{
		if (0 != SDL_GL_SetAttribute(attr, value))
			throw std::runtime_error{SDL_GetError()};
	}
	
	static WindowPtr MakeWindow(const char* title, Vec2u16 scr)
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

	const Vec3& DefaultCamera::GetPos() const noexcept
	{
		return Vec3::zero;
	}

	const Mat4& DefaultCamera::GetViewProj() const noexcept
	{
		return view_proj_;
	}

	void DefaultCamera::OnScreenSizeChanged(Vec2u16 scr)
	{
		static const auto view = MakeLookAt(Vec3::zero, Vec3::forward, Vec3::up);
		const auto proj = MakePerspective(Vec2{scr}, 10_f, 10000_f, 60_deg);
		view_proj_ = view * proj;
	}

	Renderer::Renderer(IEngine& engine, Vec2u16 scr)
		:engine_{engine}, scr_sz_{scr},
		window_{MakeWindow(engine.GetGameName().data(), scr)},
		gl_context_{CreateGlContext(*window_)},
		sprite_shader_{"../Engine/Shaders/Sprite"},
		sprite_verts_{CreateSpriteVerts()}
	{
		UnregisterCamera();
		UnregisterDirLight();
		UnregisterSkyLight();
		sprite_shader_.SetUniform(NAME("uViewProj"), MakeSimpleViewProj<4>(scr));
	}

	Renderer::~Renderer() = default;

	void Renderer::DrawScene()
	{
		gl(glClearColor, 0, 0, 0, 1);
		gl(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Draw3D();
		Draw2D();

		SDL_GL_SwapWindow(window_.get());
	}

	void Renderer::Draw3D()
	{
		gl(glEnable, GL_DEPTH_TEST);
		gl(glDisable, GL_BLEND);
		
		const auto view_proj = camera_->GetViewProj();

		for (auto& [shader_name, meshes] : mesh_comps_)
		{
			auto& shader = shaders_.at(shader_name);
			shader.Activate();
			
			shader.SetUniform(NAME("uViewProj"), view_proj);
			shader.SetUniform(NAME("uCamPos"), camera_->GetPos());
			shader.SetUniform(NAME("uSkyLight"), sky_light_->GetColor());

			const auto& dir_light = dir_light_->GetData();
			shader.SetUniform(NAME("uDirLight.dir"), dir_light.dir);
			shader.SetUniform(NAME("uDirLight.color"), dir_light.color);
			
			for (auto mesh : meshes)
			{
				if (auto info = mesh.get().Draw())
				{
					// TODO: Load specular data from mesh
					shader.SetUniform(NAME("uSpecular"), 0.2f);
					shader.SetUniform(NAME("uWorldTransform"), info->transform);
					gl(glDrawElements, GL_TRIANGLES, info->vertices, GL_UNSIGNED_SHORT, nullptr);
				}
			}
		}
	}

	void Renderer::Draw2D()
	{
		gl(glEnable, GL_BLEND);
		gl(glDisable, GL_DEPTH_TEST);
		gl(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		sprite_shader_.Activate();
		for (auto sprite : sprites_)
		{
			if (auto info = sprite.get().Draw())
			{
				sprite_shader_.SetUniform(NAME("uWorldTransform"), info->transform);
				gl(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
			}
		}
	}

	void Renderer::UnregisterDirLight() noexcept
	{
		class Def : public IDirLight
		{
		public:
			[[nodiscard]] const Data& GetData() const noexcept override
			{
				static const Data data{Vec3{1, 1, -1}.Unit(), Vec3::one};
				return data;
			}
		};

		static const Def def;
		dir_light_ = &def;
	}

	void Renderer::UnregisterSkyLight() noexcept
	{
		class Def : public ISkyLight
		{
		public:
			[[nodiscard]] const Vec3& GetColor() const noexcept override
			{
				static const Vec3 color{All{}, 0.1_f};
				return color;
			}
		};

		static const Def def;
		sky_light_ = &def;
	}

	SharedPtr<Texture> Renderer::GetTexture(Path path)
	{
		const auto found = textures_.find(path);
		if (found != textures_.end()) return found->second.lock();

		SharedPtr<Texture> loaded{
			New<Texture>(path),
			[this, path](Texture* p) noexcept
			{
				textures_.erase(path);
				Delete(p);
			}
		};

		textures_.emplace(path, loaded);
		return loaded;
	}

	SharedPtr<Mesh> Renderer::GetMesh(Path path)
	{
		const auto found = meshes_.find(path);
		if (found != meshes_.end()) return found->second.lock();

		SharedPtr<Mesh> loaded{
			New<Mesh>(path, *this),
			[this, path](Mesh* p) noexcept
			{
				meshes_.erase(path);
				Delete(p);
			}
		};

		meshes_.emplace(path, loaded);
		return loaded;
	}

	void Renderer::RegisterSprite(const ISpriteComponent& sprite)
	{
		auto cmp = [](const ISpriteComponent& a, const ISpriteComponent& b) { return a.GetDrawOrder() <= b.GetDrawOrder(); };
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void Renderer::UnregisterSprite(const ISpriteComponent& sprite)
	{
		auto pr = [&](const ISpriteComponent& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void Renderer::RegisterMesh(const IMeshComponent& mesh)
	{
		auto cmp = [](const IMeshComponent& a, const IMeshComponent& b)
		{
			auto &mat1 = a.GetMaterial(), &mat2 = b.GetMaterial();
			auto &s1 = mat1.GetShader(), &s2 = mat2.GetShader();
			if (&s1 != &s2) return &s1 < &s2;
			if (&mat1 != &mat2)
			{
				auto &t1 = mat1.GetTexture(), &t2 = mat2.GetTexture();
				if (&t1 != &t2) return &t1 < &t2;
				return &mat1 < &mat2;
			}
			auto &mesh1 = a.GetMesh(), &mesh2 = b.GetMesh();
			return &mesh1 < &mesh2;
		};
		const auto pos = std::lower_bound(mesh_comps_.begin(), mesh_comps_.end(), mesh, cmp);
		mesh_comps_.emplace(pos, mesh);
	}

	void Renderer::UnregisterMesh(const IMeshComponent& mesh)
	{
		auto pr = [&](const IMeshComponent& m) { return &m == &mesh; };
		const auto found = std::find_if(mesh_comps_.rbegin(), mesh_comps_.rend(), pr);
		if (found != mesh_comps_) mesh_comps_.erase(found.base() - 1);
	}
}
