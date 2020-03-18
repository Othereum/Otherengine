#pragma once
#include <chrono>
#include <memory>
#include <vector>

namespace Game
{
	class AActor;
	class CInputComponent;
	class CCircleComponent;
	
	class CWorld
	{
	public:
		template <class T>
		T& SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto ptr = std::make_unique<T>(*this);
			auto& actor = *ptr;
			RegisterActor(std::move(ptr));
			return actor;
		}

		void RegisterInputComponent(const CInputComponent& comp);
		void UnregisterInputComponent(const CInputComponent& comp);

		void RegisterCollision(CCircleComponent& comp);
		void UnregisterCollision(CCircleComponent& comp);
		
		[[nodiscard]] auto GetTime() const noexcept { return time_; }

	private:
		float UpdateTime();
		void RegisterActor(std::unique_ptr<AActor>&& actor);
		
		std::vector<std::reference_wrapper<const CInputComponent>> inputComps_;
		std::vector<std::reference_wrapper<CCircleComponent>> collisions_;
		
		std::vector<std::unique_ptr<AActor>> actors_;
		std::vector<std::unique_ptr<AActor>> pendingActors_;
		
		std::chrono::steady_clock::time_point time_;
	};
}
