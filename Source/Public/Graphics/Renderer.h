#pragma once
#include <memory>
#include "Rect.h"
#include "Rotation.h"

struct SDL_Texture;
struct SDL_Rect;
struct SDL_Window;

namespace oeng
{
	class CSpriteComponent;

	namespace graphics
	{
		constexpr Vector<uint16_t, 2> kScrSz{1024, 768};
		
		class CRenderer
		{
		public:
			using TWindowPtr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
			using GlContextPtr = std::unique_ptr<void, void(*)(void*)>;
		
			CRenderer();
			~CRenderer();
			
			void RegisterSprite(const CSpriteComponent& sprite);
			void UnregisterSprite(const CSpriteComponent& sprite);

			void Draw(SDL_Texture& texture, const FRect& dest, Degrees angle = {}) const;
			void Draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, Degrees angle = {}) const;

			void DrawScene() const;

		private:
			TWindowPtr window_;
			GlContextPtr gl_context_;
			std::unique_ptr<class Shader> sprite_shader_;
			std::unique_ptr<class VertexArray> sprite_verts_;
			std::vector<std::reference_wrapper<const CSpriteComponent>> sprites_;
		};
	}
}
