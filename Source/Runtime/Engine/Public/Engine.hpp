#pragma once
#include "InputSystem.hpp"
#include "Json.hpp"
#include "Renderer.hpp"
#include "World.hpp"
#include "Interfaces/Engine.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace oeng
{
	class Texture;
	class Mesh;

	class OEAPI SdlRaii
	{
	public:
		SdlRaii();
		~SdlRaii();
	};
	
	class OEAPI Engine : public IEngine, SdlRaii
	{
	public:
		OE_DELETE_CPMV(Engine);

		explicit Engine(void(*load_game)(Engine&));
		~Engine();
		
		void RunLoop();
		void Shutdown();
		
		[[nodiscard]] World& GetWorld() noexcept override { return world_; }
		[[nodiscard]] InputSystem& GetInputSystem() noexcept override { return input_system_; }
		[[nodiscard]] Renderer& GetRenderer() noexcept override { return renderer_; }
		[[nodiscard]] Vec2u16 GetWindowSize() const noexcept override;

		/**
		 * \brief Find config or create if not found.
		 * \param name Config name
		 * \return Reference to non-const json config object
		 */
		[[nodiscard]] Json& Config(Name name) override;
		
		/**
		 * \brief Save config as file. Existing files will be overwritten. Log error if fails
		 * \param name Config name
		 * \return true if successful
		 */
		bool SaveConfig(Name name) override;
		
	private:
		void Tick();
		void ProcessEvent();

		HashMap<Name, Json> configs_;
		
		Renderer renderer_;
		World world_;
		InputSystem input_system_;

		bool is_running_ = true;
	};
}
