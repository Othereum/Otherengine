#include "Components/MeshComponent.hpp"
#include <GL/glew.h>
#include "Shader.hpp"
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
		GetRenderer().UnregisterMesh(*this);
	}

	void MeshComponent::Draw() const noexcept
	{
		if (!mesh_) return;

		// TODO: Private shader
		// shader.SetTransform(GetWorldTransform());
		mesh_->GetTextures()[texture_idx_]->Activate();

		auto& va = mesh_->GetVertexArray();
		va.Activate();

		glDrawElements(GL_TRIANGLES, va.GetNumIndices() * 3, GL_UNSIGNED_SHORT, nullptr);
	}

	void MeshComponent::SetMesh(Path file)
	{
		SetMesh(GetEngine().GetMesh(file));
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
