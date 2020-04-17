#include "Actor/Ship.h"
#include <array>
#include "Actor/Asteroid.h"
#include "Actor/Laser.h"
#include "Component/SpriteComponent.h"
#include "Component/InputComponent.h"
#include "Component/PawnMoveComponent.h"
#include "Component/CircleComponent.h"
#include "World.h"
#include "Graphics/Renderer.h"
#include "TimerManager.h"

namespace game
{
	static constexpr std::array kShipPng{"Assets/Ship.png", "Assets/ShipWithThrust.png"};
	
	ship::ship(CWorld& world)
		:AActor{world}
	{
		auto& sprite = AddComponent<CSpriteComponent>();
		sprite.SetTexture(kShipPng[0]);

		auto& movement = AddComponent<CPawnMoveComp>();

		auto& col = AddComponent<CCircleComponent>();
		col.BindOnOverlap([&](CCircleComponent& c)
		{
			if (dynamic_cast<AAsteroid*>(&c.GetOwner()))
			{
				SetEnabled(false);
				
				GetTimerManager().SetTimer(2s, [&]()
				{
					SetEnabled(true);
					SetPos(graphics::kScrSz / 2);
					SetRot({});
				});
			}
		});

		auto& input = AddComponent<CInputComponent>();
		
		input.BindAxis("MoveForward", [&](float f)
		{
			const auto bShouldMove = !math::IsNearlyZero(f);
			if (bShouldMove) movement.AddInput(GetForward() * f);

			if (bShouldMove != bIsMoving_)
			{
				sprite.SetTexture(kShipPng[bShouldMove]);
				bIsMoving_ = bShouldMove;
			}
		});

		input.BindAxis("Turn", [&](float f)
		{
			movement.AddRotationInput(f);
		});

		input.BindAction("Shoot", true, [this]()
		{
			const auto cur = GetWorld().GetTime();
			if (nextAttack_ <= cur)
			{
				auto& l = GetWorld().SpawnActor<ALaser>();
				l.SetPos(GetPos());
				l.SetRot(GetRot());

				using namespace std::chrono_literals;
				nextAttack_ = cur + 500ms;
			}
		});
	}
}
