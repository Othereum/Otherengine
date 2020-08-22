#include "Components/MeshComponent.hpp"
#include "Mesh.hpp"
#include "Renderer.hpp"

namespace oeng::engine
{
	MeshComponent::MeshComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order},
		mesh_{GetRenderer().GetDefaultMesh()},
		material_{GetRenderer().GetDefaultMaterial()}
	{
	}

	MeshComponent::~MeshComponent()
	{
		if (HasBegunPlay()) GetRenderer().UnregisterMesh(*this);
	}

	void MeshComponent::SetMesh(Path file)
	{
		auto mesh = GetRenderer().GetMesh(file);
		material_ = mesh->GetMaterialShared();
		SetMesh(std::move(mesh));
	}

	void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		mesh_ = std::move(mesh);
		ReRegister();
		RecalcRadius();
	}

	void MeshComponent::SetMaterial(Path path)
	{
		SetMaterial(GetRenderer().GetMaterial(path));
	}

	void MeshComponent::SetMaterial(std::shared_ptr<Material> material)
	{
		material_ = std::move(material);
		ReRegister();
	}

	Float MeshComponent::GetUnscaledRadius() const noexcept
	{
		return mesh_->GetRadius();
	}

	void MeshComponent::OnBeginPlay()
	{
		GetRenderer().RegisterMesh(*this);
	}

	void MeshComponent::OnTrsfChanged()
	{
		RecalcRadius();
	}

	void MeshComponent::ReRegister() const
	{
		if (HasBegunPlay())
		{
			auto& renderer = GetRenderer();
			renderer.UnregisterMesh(*this);
			renderer.RegisterMesh(*this);
		}
	}

	void MeshComponent::RecalcRadius() noexcept
	{
		radius_ = Max(GetWorldScale()) * mesh_->GetRadius();
	}
}
