#include "Components/MeshComponent.hpp"
#include "IRenderer.hpp"
#include "Engine/Mesh.hpp"
#include "Materials/IMaterial.hpp"

namespace oeng
{
inline namespace engine
{
MeshComponent::MeshComponent(AActor& owner, int update_order)
    : SceneComponent{owner, update_order},
      mesh_{Mesh::GetDefault()}
{
}

MeshComponent::~MeshComponent()
{
    if (HasBegunPlay())
        IRenderer::Get().RemoveMesh(*this);
}

void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
{
    mesh_ = std::move(mesh);
    ReRegister();
}

void MeshComponent::OverrideMaterial(std::shared_ptr<IMaterial> material)
{
    material_override_ = std::move(material);
    ReRegister();
}

Float MeshComponent::GetScaledRadius() const noexcept
{
    return mesh_->GetRadius() * Max(GetWorldScale());
}

Float MeshComponent::GetUnscaledRadius() const noexcept
{
    return mesh_->GetRadius();
}

const std::shared_ptr<IMaterial>& MeshComponent::GetMaterial() const noexcept
{
    if (material_override_)
        return material_override_;

    return mesh_->GetMaterial();
}

void MeshComponent::OnBeginPlay()
{
    IRenderer::Get().AddMesh(*this);
}

void MeshComponent::ReRegister()
{
    if (HasBegunPlay())
    {
        auto& renderer = IRenderer::Get();
        renderer.RemoveMesh(*this);
        renderer.AddMesh(*this);
    }
}
}
}
