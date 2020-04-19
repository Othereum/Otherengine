#include <cmath>
#include "Components/AnimSpriteComponent.h"
#include "MathUtil.h"

namespace oeng
{
	CAnimSpriteComponent::FAnimation::FAnimation(std::initializer_list<std::shared_ptr<SDL_Texture>> textures, float fps, bool loop, float startFrame)
		:loop{loop}, cur{startFrame}, fps{fps}, textures{textures}
	{
	}

	CAnimSpriteComponent::CAnimSpriteComponent(AActor& owner, int drawOrder, int updateOrder)
		:CSpriteComponent{owner, drawOrder, updateOrder}
	{
	}

	size_t CAnimSpriteComponent::AddAnim(FAnimation&& anim)
	{
		anims_.push_back(std::move(anim));
		return anims_.size() - 1;
	}

	void CAnimSpriteComponent::Update(const float deltaSeconds)
	{
		auto& anim = anims_[idx_];
		const auto oldFrame = static_cast<size_t>(anim.cur);
		anim.cur += anim.fps * deltaSeconds;
		anim.cur = anim.loop ? fmod(anim.cur, static_cast<float>(anim.textures.size())) : math::Min(anim.cur, anim.textures.size() - 1);
		const auto newFrame = static_cast<size_t>(anim.cur);
		if (oldFrame != newFrame) SetTexture(anim.textures[newFrame]);
	}
}
