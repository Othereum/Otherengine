#pragma once
#include <memory>
#include <unordered_map>
#include "Math.hpp"
#include "Path.hpp"

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
	
	class Engine : SdlRaii
	{
	public:
		Engine();
		~Engine();
		
		void RunLoop();
		void Shutdown();
		
		[[nodiscard]] std::shared_ptr<Texture> GetTexture(Path file);
		[[nodiscard]] std::shared_ptr<Mesh> GetMesh(Path file);
		[[nodiscard]] World& GetWorld() const noexcept { return *world_; }
		[[nodiscard]] InputSystem& GetInputSystem() const noexcept { return *input_system_; }
		[[nodiscard]] Renderer& GetRenderer() const noexcept { return *renderer_; }
		[[nodiscard]] Vec2u16 GetScreenSize() const noexcept;

		Engine(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine& operator=(Engine&&) = delete;
		
	private:
		void Tick();
		void ProcessEvent();

		std::unordered_map<Path, std::weak_ptr<Texture>> textures_;
		std::unordered_map<Path, std::weak_ptr<Mesh>> meshes_;
		std::unique_ptr<Renderer> renderer_;
		std::unique_ptr<World> world_;
		std::unique_ptr<InputSystem> input_system_;
		bool is_running_ = true;
	};
}
