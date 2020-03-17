#pragma once
#include <functional>
#include <unordered_set>
#include "ActorComponent.h"

struct SDL_KeyboardEvent;

namespace Game
{
	enum class EKeyEvent { released, pressed };
	using TKey = int;

	struct FInputAction
	{
		[[nodiscard]] virtual std::unordered_set<TKey> Keys() const = 0;
	};

	struct FAxis
	{
		TKey key;
		float scale;
	};
	
	struct FInputAxis
	{
		[[nodiscard]] virtual std::vector<FAxis> Keys() const = 0;
	};

	class CInputComponent : public CActorComponent
	{
	public:
		explicit CInputComponent(class AActor& owner, int updateOrder = 100, int inputReceiveOrder = 100);
		~CInputComponent();

		void BeginPlay() override;
		void ProcessInput(const std::vector<int> (&events)[2], const uint8_t* keyboard) const;
		
		void BindAction(const FInputAction& action, EKeyEvent event, std::function<void()>&& callback);
		void BindAxis(const FInputAxis& axis, std::function<void(float)>&& callback);

		[[nodiscard]] int GetReceiveOrder() const noexcept { return receiveOrder_; }
		
	private:
		std::vector<std::pair<std::unordered_set<TKey>, std::function<void()>>> actions_[2];
		std::vector<std::pair<std::vector<FAxis>, std::function<void(float)>>> axises_;

		int receiveOrder_;
	};
}
