#include "Components/MeshComponent.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"

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
		if (auto mesh = GetRenderer().GetMesh(file))
		{
			SetMaterial(mesh->GetMaterialPtr());
			SetMesh(std::move(mesh));
		}
		else
		{
			// TODO: Better error handling
			std::terminate();
		}
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
