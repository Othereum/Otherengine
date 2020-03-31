#pragma once

#include <memory>
#include "ActorComponent.h"
#include "Vector.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace game
{
	namespace graphics
	{
		class CRenderer;
	}
	
	class CSpriteComponent : public CActorComponent
	{
	public:
		explicit CSpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
		~CSpriteComponent();

		void BeginPlay() override;
		void DrawSprite() const;
		
		void SetTexture(std::shared_ptr<SDL_Texture>&& texture);
		void SetTexture(const std::shared_ptr<SDL_Texture>& texture);
		void SetTexture(const char* filename);
		[[nodiscard]] SDL_Texture& GetTexture() const { return *texture_; }

		[[nodiscard]] const Vector2<unsigned short>& GetTexSize() const noexcept { return tex_size_; }
		[[nodiscard]] int GetDrawOrder() const noexcept { return draw_order_; }
		[[nodiscard]] graphics::CRenderer& GetRenderer() const noexcept;

	private:
		virtual void Draw() const;

		std::shared_ptr<SDL_Texture> texture_;
		Vector2<uint16_t> tex_size_;
		int draw_order_;
	};
}