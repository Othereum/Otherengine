#pragma once
#include <vector>
#include "SpriteComponent.h"

namespace game
{
	class CBgSpriteComponent : public CSpriteComponent
	{
	public:
		explicit CBgSpriteComponent(AActor& owner, int drawOrder = 10, int updateOrder = 100);
		
		void Update(float deltaSeconds) override;
		void Draw() const override;

		void SetBgTextures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);
		void AddBgTextures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);

		void SetScrollSpeed(float speed) { scrollSpeed_ = speed; }
		[[nodiscard]] float GetScrollSpeed() const { return scrollSpeed_; }

	private:
		struct FBgTexture
		{
			std::shared_ptr<SDL_Texture> texture;
			FVector2 offset;
		};
		std::vector<FBgTexture> textures_;
		float scrollSpeed_{};
	};
}
