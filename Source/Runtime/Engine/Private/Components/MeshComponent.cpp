#include "Components/MeshComponent.hpp"
#include "IRenderer.hpp"
#include "Engine/Mesh.hpp"
#include "Materials/IMaterial.hpp"

namespace oeng
{
inline namespace engine
{
MeshComponent::MeshComponent()
    : mesh_{Mesh::GetDefault()}
{
}

void MeshComponent::SetMesh(SharedRef<Mesh> mesh)
{
    mesh_ = std::move(mesh);
    ReRegister();
}

void MeshComponent::OverrideMaterial(SharedRef<IMaterial> material)
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

const SharedRef<IMaterial>& MeshComponent::GetMaterial() const noexcept
{
    if (material_override_)
        return material_override_;

    return mesh_->GetMaterial();
}

void MeshComponent::OnBeginPlay()
{
    IRenderer::Get().AddMesh(*this);
}

void MeshComponent::OnEndPlay()
{
    IRenderer::Get().RemoveMesh(*this);
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
