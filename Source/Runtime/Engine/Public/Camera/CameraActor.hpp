#pragma once
#include "Actors/Actor.hpp"

namespace oeng::engine
{
	class CameraComponent;
	
	class ENGINE_API ACameraActor : public AActor
	{
	public:
		explicit ACameraActor(World& world);

		void Activate() const noexcept;

		void SetVFov(Rad vfov) const noexcept;
		void SetNearFar(Float near, Float far) const noexcept;
		
		[[nodiscard]] Rad GetVFov() const noexcept;
		[[nodiscard]] Float GetNear() const noexcept;
		[[nodiscard]] Float GetFar() const noexcept;

		[[nodiscard]] CameraComponent& GetCameraComponent() const noexcept { return camera_; }
		
	private:
		CameraComponent& camera_;
	};
}