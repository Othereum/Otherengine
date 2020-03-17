#include <array>
#include "Actor/Ship.h"
#include "Actor/Laser.h"
#include "Component/SpriteComponent.h"
#include "Component/InputComponent.h"
#include "Component/PawnMoveComponent.h"
#include "Engine.h"

namespace Game
{
	static constexpr std::array kShipPng{"Assets/Ship.png", "Assets/ShipWithThrust.png"};
	
	struct FInputForward : FInputAxis
	{
		[[nodiscard]] std::vector<FAxis> Keys() const override
		{
			return {{'w', 1}, {'s', -1}};
		}
	};
	
	struct FInputRotate : FInputAxis
	{
		[[nodiscard]] std::vector<FAxis> Keys() const override
		{
			return {{'a', -1}, {'d', 1}};
		}
	};

	struct FInputShoot : FInputAction
	{
		[[nodiscard]] std::unordered_set<TKey> Keys() const override
		{
			return {' '};
		}
	};

	ship::ship(CEngine& app)
		:AActor{app}
	{
		auto& sprite = AddComponent<CSpriteComponent>();
		sprite.SetTexture(kShipPng[0]);

		auto& movement = AddComponent<CPawnMoveComp>();

		auto& input = AddComponent<CInputComponent>();
		
		input.BindAxis(FInputForward{}, [&](float f)
		{
			const auto bShouldMove = !Math::IsNearlyZero(f);
			if (bShouldMove) movement.AddInput(GetForward() * f);

			if (bShouldMove != bIsMoving_)
			{
				sprite.SetTexture(kShipPng[bShouldMove]);
				bIsMoving_ = bShouldMove;
			}
		});

		input.BindAxis(FInputRotate{}, [&](float f)
		{
			movement.AddRotationInput(f);
		});

		input.BindAction(FInputShoot{}, EKeyEvent::pressed, [this]()
		{
			auto& l = GetApp().SpawnActor<ALaser>();
			l.SetPos(GetPos());
			l.SetRot(GetRot());
		});
	}
}
