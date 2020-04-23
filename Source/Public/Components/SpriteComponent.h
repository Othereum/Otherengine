#pragma once

#include <memory>
#include "ActorComponent.h"

struct SDL_Renderer;

namespace oeng
{
	namespace graphics
	{
		class CRenderer;
		class Texture;
	}
	
	class CSpriteComponent : public CActorComponent
	{
	public:
		explicit CSpriteComponent(AActor& owner, int draw_order = 100, int update_order = 100);
		~CSpriteComponent();

		void BeginPlay() override;
		
		void SetTexture(std::shared_ptr<graphics::Texture>&& texture);
		void SetTexture(const std::shared_ptr<graphics::Texture>& texture);
		void SetTexture(const char* filename);
		[[nodiscard]] graphics::Texture& GetTexture() const { return *texture_; }

		[[nodiscard]] int GetDrawOrder() const noexcept { return draw_order_; }
		[[nodiscard]] graphics::CRenderer& GetRenderer() const noexcept;

	private:
		std::shared_ptr<graphics::Texture> texture_;
		int draw_order_;
	};
}