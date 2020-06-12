#include "Components/MeshComponent.hpp"
#include "Engine.hpp"

namespace oeng
{
	MeshComponent::MeshComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}
	{
	}

	MeshComponent::~MeshComponent()
	{
		if (HasBegunPlay()) GetRenderer().UnregisterMesh(*this);
	}

	void MeshComponent::SetMesh(Path file)
	{
		SetMesh(GetRenderer().GetMesh(file));
	}

	void MeshComponent::SetMaterial(Path path)
	{
		SetMaterial(GetRenderer().GetMaterial(path));
	}

	Renderer& MeshComponent::GetRenderer() const noexcept
	{
		return GetEngine().GetRenderer();
	}

	void MeshComponent::OnBeginPlay()
	{
		GetRenderer().RegisterMesh(*this);
	}
}
