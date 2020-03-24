#include "Actor/Asteroid.h"
#include "Component/MoveComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/CircleComponent.h"
#include "MathUtil.h"

namespace game
{
	AAsteroid::AAsteroid(CWorld& world)
		:AActor{world}
	{
		auto& sprite = AddComponent<CSpriteComponent>();
		sprite.SetTexture("Assets/Asteroid.png");

		auto& movement = AddComponent<CMoveComponent>();
		movement.SetVelocity(math::RandUnitVec() * 150);

		AddComponent<CCircleComponent>();
	}
}
