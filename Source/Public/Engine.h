#pragma once

#include <memory>
#include <unordered_map>
#include "Name.h"
#include "Math.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Rect;

namespace oeng
{
	namespace graphics
	{
		class CRenderer;
		class Texture;
	}
	
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
		
		[[nodiscard]] std::shared_ptr<graphics::Texture> GetTexture(Name file);
		[[nodiscard]] CWorld& GetWorld() const noexcept { return *world_; }
		[[nodiscard]] CInputSystem& GetInputSystem() const noexcept { return *input_system_; }
		[[nodiscard]] graphics::CRenderer& GetRenderer() const noexcept { return *renderer_; }
		[[nodiscard]] const Vector<uint16_t, 2>& GetScreenSize() const noexcept;
		
	private:
		void Tick();
		void ProcessEvent();

		std::unordered_map<Name, std::weak_ptr<graphics::Texture>> textures_;
		std::unique_ptr<graphics::CRenderer> renderer_;
		std::unique_ptr<CWorld> world_;
		std::unique_ptr<CInputSystem> input_system_;
		bool is_running_ = true;
	};
}
