#include "Actor/Asteroid.h"
#include "Component/MoveComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/CircleComponent.h"
#include "MathUtil.h"

namespace Game
{
	AAsteroid::AAsteroid(CEngine& app)
		:AActor{app}
	{
		auto& sprite = AddComponent<CSpriteComponent>();
		sprite.SetTexture("Assets/Asteroid.png");

		auto& movement = AddComponent<CMoveComponent>();
		movement.SetVelocity(Math::RandUnitVec() * 150);

		AddComponent<CCircleComponent>();
	}
}
