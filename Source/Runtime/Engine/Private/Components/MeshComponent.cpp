#include "Components/MeshComponent.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "VertexArray.hpp"
#include "Engine.hpp"

namespace oeng
{
	MeshComponent::MeshComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}
	{
	}

	MeshComponent::~MeshComponent()
	{
		if (BegunPlay()) GetRenderer().UnregisterMesh(*this);
	}

	std::optional<IMesh::DrawInfo> MeshComponent::Draw() const noexcept
	{
		if (!IsEnabled()) return {};

		mesh_->GetTextures()[texture_idx_]->Activate();

		auto& va = mesh_->GetVertexArray();
		va.Activate();

		return DrawInfo{GetWorldTransform(), va.GetNumIndices() * 3};
	}

	Path MeshComponent::GetShaderPath() const noexcept
	{
		return mesh_->GetShaderPath();
	}

	void MeshComponent::SetMesh(Path file)
	{
		SetMesh(GetRenderer().GetMesh(file));
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
