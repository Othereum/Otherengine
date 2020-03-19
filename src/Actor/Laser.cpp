#include "Actor/Laser.h"
#include "Actor/Asteroid.h"
#include "Component/SpriteComponent.h"
#include "Component/CircleComponent.h"

namespace Game
{
	ALaser::ALaser(CWorld& world)
		:AActor{world}
	{
		SetLifespan(1);

		auto& sprite = AddComponent<CSpriteComponent>();
		sprite.SetTexture("Assets/Laser.png");

		auto& col = AddComponent<CCircleComponent>();
		col.SetRadius(16);
		col.BindOnOverlap([this](CCircleComponent& o)
		{
			if (auto a = dynamic_cast<AAsteroid*>(&o.GetOwner()))
			{
				Destroy();
				a->Destroy();
			}
		});
	}

	void ALaser::UpdateActor(float deltaSeconds)
	{
		SetPos(GetPos() + GetForward() * (800 * deltaSeconds));
	}
}
