#pragma once
#include <vector>
#include "SpriteComponent.h"

namespace game
{
	class CAnimSpriteComponent : public CSpriteComponent
	{
	public:
		struct FAnimation
		{
			FAnimation(std::initializer_list<std::shared_ptr<SDL_Texture>> textures, float fps = 24, bool loop = true, float startFrame = 0);
			
			bool loop;
			float cur;
			float fps;
			std::vector<std::shared_ptr<SDL_Texture>> textures;
		};
		
		explicit CAnimSpriteComponent(AActor& owner, int drawOrder = 100, int updateOrder = 100);
		
		size_t AddAnim(FAnimation&& anim);
		void Play(size_t idx, bool reset = true) { idx_ = idx; if (reset) Play(); }
		void Play() { anims_[idx_].cur = 0; }

	private:
		void Update(float deltaSeconds) override;

		size_t idx_ = 0;
		std::vector<FAnimation> anims_;
	};
}
