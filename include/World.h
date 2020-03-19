#pragma once
#include <chrono>
#include <memory>
#include <vector>

union SDL_Event;

namespace Game
{
	class CEngine;
	class CRenderer;
	class AActor;
	class CInputComponent;
	class CCircleComponent;
	
	class CWorld
	{
	public:
		explicit CWorld(CEngine& engine);
		
		template <class T>
		T& SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto ptr = std::make_unique<T>(*this);
			auto& actor = *ptr;
			RegisterActor(std::move(ptr));
			return actor;
		}

		void Tick();

		void RegisterInputComponent(const CInputComponent& comp);
		void UnregisterInputComponent(const CInputComponent& comp);

		void RegisterCollision(CCircleComponent& comp);
		void UnregisterCollision(CCircleComponent& comp);
		
		[[nodiscard]] CRenderer& GetRenderer() const noexcept { return *renderer_; }
		[[nodiscard]] auto GetTime() const noexcept { return time_; }
		[[nodiscard]] CEngine& GetEngine() const noexcept { return engine_; }

	private:
		void UpdateGame();
		float UpdateTime();
		void RegisterActor(std::unique_ptr<AActor>&& actor);

		CEngine& engine_;
		std::unique_ptr<CRenderer> renderer_;

		std::vector<std::reference_wrapper<CCircleComponent>> collisions_;
		
		std::vector<std::unique_ptr<AActor>> actors_;
		std::vector<std::unique_ptr<AActor>> pendingActors_;
		
		std::chrono::steady_clock::time_point time_;
	};
}
