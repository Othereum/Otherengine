#pragma once
#include "InputSystem.hpp"
#include "Json.hpp"
#include "Renderer.hpp"
#include "World.hpp"
#include "Interfaces/IEngine.hpp"

namespace oeng::engine
{
	class OEAPI SdlRaii
	{
	public:
		SdlRaii();
		~SdlRaii();
	};
	
	class OEAPI Engine : public IEngine, SdlRaii
	{
	public:
		DELETE_CPMV(Engine);

		explicit Engine(void load_game(Engine&));
		~Engine();
		
		void RunLoop();
		void Shutdown();
		
		[[nodiscard]] World& GetWorld() noexcept override { return world_; }
		[[nodiscard]] InputSystem& GetInputSystem() noexcept override { return input_system_; }
		[[nodiscard]] Renderer& GetRenderer() noexcept override { return renderer_; }
		[[nodiscard]] Json& Config(Name name) override;
		bool SaveConfig(Name name) override;
		
	private:
		void Tick();
		void ProcessEvent();

		HashMap<Name, Json> configs_;
		
		Renderer renderer_;
		InputSystem input_system_;
		World world_;

		uint64_t ticks_ = 0;
		bool is_running_ = true;
	};
}
