#pragma once

#include <memory>
#include <unordered_map>
#include "Name.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

namespace oeng
{
	class CWorld;
	class CInputSystem;

	class CSdlRaii
	{
	public:
		CSdlRaii();
		~CSdlRaii();
	};
	
	class CEngine : CSdlRaii
	{
	public:
		CEngine();
		~CEngine();
		
		void RunLoop();
		void Shutdown();
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> GetTexture(FName file);
		[[nodiscard]] CWorld& GetWorld() const noexcept { return *world_; }
		[[nodiscard]] const CInputSystem& GetInputSystem() const noexcept { return *input_system_; }
		
	private:
		void Tick();
		void ProcessEvent();
		[[nodiscard]] std::shared_ptr<SDL_Texture> LoadTexture(FName file);

		std::unordered_map<FName, std::weak_ptr<SDL_Texture>> textures_;
		std::unique_ptr<CWorld> world_;
		std::unique_ptr<CInputSystem> input_system_;
		bool is_running_ = true;
	};
}
