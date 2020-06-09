#include "Components/SpriteComponent.hpp"
#include "GameFramework/Actor.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace oeng
{
	SpriteComponent::SpriteComponent(AActor& owner, const int draw_order, const int update_order)
		:ActorComponent{owner, update_order}, draw_order_{draw_order}
	{
	}

	SpriteComponent::~SpriteComponent()
	{
		GetRenderer().UnregisterSprite(*this);
	}

	std::optional<ISprite::DrawInfo> SpriteComponent::Draw() const noexcept
	{
		if (!IsEnabled()) return {};
		
		texture_->Activate();
		return DrawInfo{MakeScale<4>({texture_->Size(), 1}) * GetOwner().GetTransformMatrix()};
	}

	void SpriteComponent::SetTexture(Path file)
	{
		SetTexture(GetRenderer().GetTexture(file));
	}

	Renderer& SpriteComponent::GetRenderer() const noexcept
	{
		return GetEngine().GetRenderer();
	}

	void SpriteComponent::OnBeginPlay()
	{
		GetRenderer().RegisterSprite(*this);
	}
}
