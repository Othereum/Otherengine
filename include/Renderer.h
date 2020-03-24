#pragma once
#include <memory>
#include "Rect.h"
#include "Rotation.h"
#include "Vector.h"

struct SDL_Texture;
struct SDL_Rect;
struct SDL_Renderer;
struct SDL_Window;

namespace game
{
	class CSpriteComponent;
	
	constexpr Vector2<uint16_t> kScrSz{1024, 768};
	
	class CRenderer
	{
	public:
		using TWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
		using TRendererPtr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	
		CRenderer();
		
		void RegisterSprite(const CSpriteComponent& sprite);
		void UnregisterSprite(const CSpriteComponent& sprite);

		void Draw(SDL_Texture& texture, const TFRect& dest, Degrees angle = {}) const;
		void Draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, Degrees angle = {}) const;

		void DrawScene() const;

		[[nodiscard]] SDL_Renderer& GetSdlRenderer() const noexcept { return *renderer_; }

	private:
		TWindowPtr window_;
		TRendererPtr renderer_;

		std::vector<std::reference_wrapper<const CSpriteComponent>> sprites_;
	};
}