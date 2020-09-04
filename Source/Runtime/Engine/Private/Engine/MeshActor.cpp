#include "Engine/MeshActor.hpp"
#include "Components/MeshComponent.hpp"

namespace oeng
{
inline namespace engine
{
AMeshActor::AMeshActor()
    : mesh_{AddComponent<MeshComponent>()}
{
    SetRootComponent(&mesh_);
}

void AMeshActor::SetMesh(std::shared_ptr<Mesh> mesh) const
{
    mesh_.SetMesh(std::move(mesh));
}

void AMeshActor::OverrideMaterial(std::shared_ptr<IMaterial> material) const
{
    mesh_.OverrideMaterial(std::move(material));
}

const std::shared_ptr<Mesh>& AMeshActor::GetMesh() const noexcept
{
    return mesh_.GetMesh();
}

const std::shared_ptr<IMaterial>& AMeshActor::GetMaterial() const noexcept
{
    return mesh_.GetMaterial();
}
}
}
