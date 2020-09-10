#include "Components/MeshComponent.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Mesh.hpp"
#include "IRenderer.hpp"
#include "Materials/IMaterial.hpp"

namespace oeng
{
inline namespace engine
{
MeshComponent::MeshComponent() : mesh_{Mesh::GetDefault()}
{
}

void MeshComponent::SetMesh(SharedRef<Mesh> mesh)
{
    mesh_ = std::move(mesh);
    ReRegister();
}

void MeshComponent::OverrideMaterial(SharedPtr<IMaterial> material)
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

SharedRef<IMaterial> MeshComponent::GetMaterial() const noexcept
{
    if (material_override_)
        return SharedRef{material_override_};

    return mesh_->GetMaterial();
}

void MeshComponent::OnBeginPlay()
{
    GetRenderer().AddMesh(*this);
}

void MeshComponent::OnEndPlay()
{
    GetRenderer().RemoveMesh(*this);
}

void MeshComponent::ReRegister()
{
    if (HasBegunPlay())
    {
        auto& renderer = GetRenderer();
        renderer.RemoveMesh(*this);
        renderer.AddMesh(*this);
    }
}

IRenderer& MeshComponent::GetRenderer() const noexcept
{
    return GetEngine().GetRenderer();
}

} // namespace engine
} // namespace oeng
