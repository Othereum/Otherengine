#pragma once
#include "Interfaces/Engine.hpp"
#include "InputSystem.hpp"
#include "Math.hpp"
#include "Json.hpp"
#include "World.hpp"
#include "Renderer.hpp"
#include "Templates/Function.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace oeng
{
	class Texture;
	class Mesh;

	class SdlRaii
	{
	public:
		SdlRaii();
		~SdlRaii();
	};
	
	class OEAPI Engine : public IEngine, SdlRaii
	{
	public:
		OE_DELETE_CPMV(Engine);
		
		Engine(std::string_view game_name, const Function<void(Engine&)>& load_game);
		~Engine();
		
		void RunLoop();
		void Shutdown();
		
		[[nodiscard]] World& GetWorld() noexcept override { return world_; }
		[[nodiscard]] InputSystem& GetInputSystem() noexcept override { return input_system_; }
		[[nodiscard]] Renderer& GetRenderer() noexcept override { return renderer_; }
		[[nodiscard]] Vec2u16 GetWindowSize() const noexcept override;
		[[nodiscard]] std::string_view GetGameName() const noexcept override { return game_name_; }

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
		bool SaveConfig(Name name) noexcept override;
		
	private:
		void Tick();
		void ProcessEvent();

		std::string_view game_name_;
		HashMap<Name, Json> configs_;
		
		Renderer renderer_;
		World world_;
		InputSystem input_system_;

		bool is_running_ = true;
	};
}
