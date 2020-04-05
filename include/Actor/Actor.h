#pragma once

#include <memory>
#include <vector>
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include "Rotation.h"

namespace game
{
	using namespace glm;
	
	class CEngine;
	class CWorld;
	class CActorComponent;
	class CTimerManager;
	
	class AActor
	{
	public:
		enum class EState
		{
			active, paused, dead
		};

		explicit AActor(CWorld& world);
		virtual ~AActor();

		void BeginPlay() const;
		void Update(float delta_seconds);
		void Destroy();

		template <class T, class... Args>
		T& AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<CActorComponent, T>);
			auto ptr = std::make_unique<T>(*this, std::forward<Args>(args)...);
			auto& ref = *ptr;
			RegisterComponent(std::move(ptr));
			return ref;
		}

		void SetEnabled(bool enable);

		void SetLifespan(float seconds) { lifespan_ = seconds; }
		[[nodiscard]] EState GetState() const noexcept { return state_; }
		
		void SetPos(const vec2& new_pos, bool recompute_world_transform = true) noexcept;
		[[nodiscard]] const vec2& GetPos() const noexcept { return pos_; }

		void SetRot(Degrees new_rot, bool recompute_world_transform = true) noexcept;
		[[nodiscard]] Degrees GetRot() const noexcept { return rot_; }
		[[nodiscard]] vec2 GetForward() const noexcept;

		void SetScale(const vec2& scale, bool recompute_world_transform = true) noexcept;
		[[nodiscard]] const vec2& GetScale() const noexcept { return scale_; }

		void RecomputeWorldTransform() noexcept;

		[[nodiscard]] CEngine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept { return world_; }
		[[nodiscard]] CTimerManager& GetTimerManager() const noexcept;

	private:
		void RegisterComponent(std::unique_ptr<CActorComponent>&& comp);
		void UpdateComponents(float delta_seconds);
		void UpdateLifespan(float delta_seconds);
		virtual void UpdateActor(float delta_seconds) {}

		EState state_ = EState::active;
		float lifespan_ = 0;
		
		vec2 pos_;
		Degrees rot_;
		vec2 scale_;

		mat4 world_transform_;
		
		CWorld& world_;
		std::vector<std::unique_ptr<CActorComponent>> comps_;
	};
}