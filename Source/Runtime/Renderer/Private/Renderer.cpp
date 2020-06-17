#include "Renderer.hpp"

#include <stdexcept>
#include <SDL.h>

#include "OpenGL.hpp"
#include "Json.hpp"

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
	
	static int LoadDisplayIdx(Json& config)
	{
		const auto num_dp = SDL_GetNumVideoDisplays();
		if (num_dp <= 0) throw std::runtime_error{"No display detected"};
		
		auto& dp_ref = config.at("Display");
		auto dp = dp_ref.get<int>();
		if (dp >= num_dp)
		{
			log::Warn("Attempted to use a non-existent display (tried: {}, max: {})", dp, num_dp-1);
			log::Warn("Using display 0...");
			dp_ref = 0, dp = 0;
		}

		return dp;
	}

	static int LoadDisplayMode(int dp, Json& config)
	{
		const auto num_dm = SDL_GetNumDisplayModes(dp);
		if (num_dm <= 0) throw std::runtime_error{"No display mode detected"};

		auto& modes = config["DisplayModesReadOnly"];
		modes = {};
		for (auto i=0; i<num_dm; ++i)
		{
			SDL_DisplayMode dm;
			SDL_GetDisplayMode(dp, i, &dm);
			modes.emplace_back(format("[{}] {}x{} {}Hz", i, dm.w, dm.h, dm.refresh_rate));
		}

		auto& dm_ref = config.at("DisplayMode");
		auto dm = dm_ref.get<int>();
		if (dm >= num_dm)
		{
			const auto& display = modes.at(0);
			log::Warn("Attempted to use a non-existent display mode (tried: {}, max: {})", dm, num_dm-1);
			log::Warn("Using display mode {}", display.get<std::string>());
			dm_ref = 0, dm = 0;
		}

		return dm;
	}

	static WindowPtr MakeWindow(IEngine& engine)
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

		const Name config_name = "Display";
		auto& config = engine.Config(config_name);
		const auto dp = LoadDisplayIdx(config);
		const auto dm = LoadDisplayMode(dp, config);
		const auto fs = config.at("Fullscreen").get<bool>();
		engine.SaveConfig(config_name);

		SDL_DisplayMode display;
		SDL_GetDisplayMode(dp, dm, &display);

		uint32_t flags = SDL_WINDOW_OPENGL;
		if (fs) flags |= SDL_WINDOW_FULLSCREEN;

		WindowPtr window{
			SDL_CreateWindow(engine.GetGameName().data(),
				SDL_WINDOWPOS_CENTERED_DISPLAY(dp), SDL_WINDOWPOS_CENTERED_DISPLAY(dp),
				display.w, display.h, flags),
			&SDL_DestroyWindow
		};
		
		if (!window) throw std::runtime_error{SDL_GetError()};
		SDL_SetWindowDisplayMode(window.get(), &display);
		
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

	Renderer::Renderer(IEngine& engine)
		:engine_{engine}, 
		window_{MakeWindow(engine)},
		gl_context_{CreateGlContext(*window_)},
		sprite_shader_{"../Engine/Shaders/Sprite"},
		sprite_verts_{CreateSpriteVerts()},
		materials_{shaders_.default_obj, textures_.default_obj},
		meshes_{materials_.default_obj}
	{
		UnregisterCamera();
		UnregisterDirLight();
		UnregisterSkyLight();
		sprite_shader_.SetUniform("uViewProj", MakeSimpleViewProj<4>(GetWindowSize()));
	}

	Renderer::~Renderer() = default;

	void Renderer::DrawScene()
	{
		gl(glClearColor, 0, 0, 0, 1);
		gl(glClear, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto try_draw = [](auto name, auto draw)
		{
			try { draw(); }
			catch (const std::exception& e)
			{
				OE_DLOG(1s, log::level::err, "Error occured while drawing '{}': {}", name, e.what());
			}
		};

		try_draw("3D scene", [&]{ Draw3D(); });
		try_draw("2D scene", [&]{ Draw2D(); });

		SDL_GL_SwapWindow(window_.get());
	}

	void Renderer::Draw3D()
	{
		gl(glEnable, GL_DEPTH_TEST);
		gl(glDisable, GL_BLEND);

		Shader* prev_shader = nullptr;
		Material* prev_material = nullptr;
		Texture* prev_texture = nullptr;
		Mesh* prev_mesh = nullptr;
		
		const auto view_proj = camera_->GetViewProj();

		for (auto mesh_comp_ref : mesh_comps_) try
		{
			const auto& mesh_comp = mesh_comp_ref.get();
			if (!mesh_comp.ShouldDraw()) continue;
			
			auto& material = mesh_comp.GetMaterial();
			auto& shader = material.GetShader();
			if (&shader != prev_shader)
			{
				shader.Activate();
				const auto& dir_light = dir_light_->GetData();
				shader.TryUniform(NAME("uDirLight.dir"), dir_light.dir);
				shader.TryUniform(NAME("uDirLight.color"), dir_light.color);
				shader.TryUniform(NAME("uViewProj"), view_proj);
				shader.TryUniform(NAME("uCamPos"), camera_->GetPos());
				shader.TryUniform(NAME("uSkyLight"), sky_light_->GetColor());
				prev_shader = &shader;
			}

			if (&material != prev_material)
			{
				material.TryUniforms();
				prev_material = &material;
			}

			auto& texture = material.GetTexture();
			if (&texture != prev_texture)
			{
				texture.Activate();
				prev_texture = &texture;
			}

			auto& mesh = mesh_comp.GetMesh();
			auto& verts = mesh.GetVertexArray();
			if (&mesh != prev_mesh)
			{
				verts.Activate();
				prev_mesh = &mesh;
			}
			
			shader.TryUniform(NAME("uWorldTransform"), mesh_comp.GetDrawTrsf());
			gl(glDrawElements, GL_TRIANGLES, verts.GetNumIndices() * 3, GL_UNSIGNED_SHORT, nullptr);
		}
		catch (const std::exception& e)
		{
			const auto stem = mesh_comp_ref.get().GetMesh().GetStem();
			OE_DLOG(1s, log::level::err, "Failed to draw mesh '{}': {}", *stem, e.what());
		}
	}

	void Renderer::Draw2D()
	{
		gl(glEnable, GL_BLEND);
		gl(glDisable, GL_DEPTH_TEST);
		gl(glBlendFunc, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		sprite_shader_.Activate();
		for (auto sprite_ref : sprites_) try
		{
			const auto& sprite = sprite_ref.get();
			if (!sprite.ShouldDraw()) continue;

			sprite_shader_.SetUniform(NAME("uWorldTransform"), sprite.GetDrawTrsf());
			gl(glDrawElements, GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
		}
		catch (const std::exception& e)
		{
			const auto stem = sprite_ref.get().GetTexture().GetStem();
			OE_DLOG(1s, log::level::err, "Failed to draw sprite '{}': {}", *stem, e.what());
		}
	}

	void Renderer::UnregisterDirLight() noexcept
	{
		class Def : public IDirLight
		{
		public:
			[[nodiscard]] const Data& GetData() const noexcept override
			{
				static const Data data{*Vec3{1, 1, -1}.Unit(), Vec3::one};
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

	template <class T, class... Args>
	SharedRef<T> Get(Renderer::Cache<T>& cache, Path path, Args&&... args)
	{
		auto& map = cache.map;
		const auto found = map.find(path);
		if (found != map.end()) return SharedRef<T>{found->second};

		try
		{
			SharedRef<T> loaded(New<T>(path, std::forward<Args>(args)...),
				[&map, path](T* p) { map.erase(path); Delete(p); });
			
			map.emplace(path, loaded);
			return loaded;
		}
		catch (const std::exception& e)
		{
			log::Error("Failed to load '{}': {}", path->string(), e.what());
			return cache.default_obj;
		}
	}

	SharedRef<Texture> Renderer::GetTexture(Path path)
	{
		return Get(textures_, path);
	}

	SharedRef<Mesh> Renderer::GetMesh(Path path)
	{
		return Get(meshes_, path, *this);
	}

	SharedRef<Shader> Renderer::GetShader(Path path)
	{
		return Get(shaders_, path);
	}

	SharedRef<Material> Renderer::GetMaterial(Path path)
	{
		return Get(materials_, path, *this);
	}

	Vec2u16 Renderer::GetWindowSize() const noexcept
	{
		int w, h;
		SDL_GetWindowSize(window_.get(), &w, &h);
		return Vec2u16(w, h);
	}

	template <class T, class Compare>
	void Register(Renderer::CompArr<T>& arr, const T& obj, Compare cmp)
	{
		const auto pos = std::upper_bound(arr.begin(), arr.end(), obj, cmp);
		arr.emplace(pos, obj);
	}

	template <class T>
	void Unregister(Renderer::CompArr<T>& arr, const T& obj)
	{
		const auto cmp = [&obj](const T& x) { return &x == &obj; };
		const auto found = std::find_if(arr.rbegin(), arr.rend(), cmp);
		if (found != arr.rend()) arr.erase(found.base() - 1);
	}

	void Renderer::RegisterSprite(const ISpriteComponent& sprite)
	{
		Register(sprites_, sprite, [](const ISpriteComponent& a, const ISpriteComponent& b)
		{
			return a.GetDrawOrder() < b.GetDrawOrder();
		});
	}

	void Renderer::RegisterMesh(const IMeshComponent& mesh)
	{
		// Group mesh components in order of importance for [Shader -> Materials -> Texture -> Mesh]
		Register(mesh_comps_, mesh, [](const IMeshComponent& a, const IMeshComponent& b)
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
		});
	}

	void Renderer::UnregisterSprite(const ISpriteComponent& sprite)
	{
		Unregister(sprites_, sprite);
	}

	void Renderer::UnregisterMesh(const IMeshComponent& mesh)
	{
		Unregister(mesh_comps_, mesh);
	}

	void Renderer::UnregisterPointLight(const IPointLight& light)
	{
		Unregister(point_lights_, light);
	}

	void Renderer::UnregisterSpotLight(const ISpotLight& light)
	{
		Unregister(spot_lights_, light);
	}
}
