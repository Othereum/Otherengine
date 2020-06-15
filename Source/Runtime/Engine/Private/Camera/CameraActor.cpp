#include "Camera/CameraActor.hpp"
#include "Camera/CameraComponent.hpp"

namespace oeng
{
	ACameraActor::ACameraActor(World& world)
		:AActor{world}, camera_{AddComponent<CameraComponent>()}
	{
		SetRootComponent(&camera_);
	}

	void ACameraActor::Activate() const noexcept
	{
		camera_.Activate();
	}

	void ACameraActor::SetVFov(Rad vfov) const noexcept
	{
		camera_.SetVFov(vfov);
	}

	void ACameraActor::SetNearFar(Float near, Float far) const noexcept
	{
		camera_.SetNearFar(near, far);
	}

	Rad ACameraActor::GetVFov() const noexcept
	{
		return camera_.GetVFov();
	}

	Float ACameraActor::GetNear() const noexcept
	{
		return camera_.GetNear();
	}

	Float ACameraActor::GetFar() const noexcept
	{
		return camera_.GetFar();
	}
}
