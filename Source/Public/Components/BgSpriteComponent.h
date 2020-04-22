#pragma once
#include <vector>
#include "SpriteComponent.h"

namespace oeng
{
	class CBgSpriteComponent : public CSpriteComponent
	{
	public:
		explicit CBgSpriteComponent(AActor& owner, int draw_order = 10, int update_order = 100);
		
		void Update(float delta_seconds) override;
		void Draw(const graphics::CRenderer& renderer) const override;

		void SetBgTextures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);
		void AddBgTextures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);

		void SetScrollSpeed(float speed) { scroll_speed_ = speed; }
		[[nodiscard]] float GetScrollSpeed() const { return scroll_speed_; }

	private:
		struct FBgTexture
		{
			std::shared_ptr<SDL_Texture> texture;
			Vec2 offset;
		};
		std::vector<FBgTexture> textures_;
		float scroll_speed_{};
	};
}
