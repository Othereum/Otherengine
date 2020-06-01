#pragma once
#include "API.hpp"
#include "Math.hpp"
#include "Path.hpp"
#include "Templates/HashMap.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace oeng
{
	class Renderer;
	class Texture;
	class Mesh;
	class World;
	class InputSystem;

	class SdlRaii
	{
	public:
		SdlRaii();
		~SdlRaii();
	};
	
	class OEAPI Engine : SdlRaii
	{
	public:
		Engine(std::string_view game_name, const std::function<void(Engine&)>& load_game);
		~Engine();
		
		void RunLoop();
		void Shutdown();
		
		[[nodiscard]] SharedPtr<Texture> GetTexture(Path file);
		[[nodiscard]] SharedPtr<Mesh> GetMesh(Path file);
		[[nodiscard]] World& GetWorld() const noexcept { return *world_; }
		[[nodiscard]] InputSystem& GetInputSystem() const noexcept { return *input_system_; }
		[[nodiscard]] Renderer& GetRenderer() const noexcept { return *renderer_; }
		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept;
		[[nodiscard]] std::string_view GetGameName() const noexcept { return game_name_; }

		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;
		
	private:
		void Tick();
		void ProcessEvent();

		std::string_view game_name_;
		HashMap<Path, WeakPtr<Texture>> textures_;
		HashMap<Path, WeakPtr<Mesh>> meshes_;
		UniquePtr<Renderer> renderer_;
		UniquePtr<World> world_;
		UniquePtr<InputSystem> input_system_;
		bool is_running_ = true;
	};
}
