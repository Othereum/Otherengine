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

void AMeshActor::SetMesh(SharedRef<Mesh> mesh) const
{
    mesh_.SetMesh(std::move(mesh));
}

void AMeshActor::OverrideMaterial(SharedRef<IMaterial> material) const
{
    mesh_.OverrideMaterial(std::move(material));
}

const SharedRef<Mesh>& AMeshActor::GetMesh() const noexcept
{
    return mesh_.GetMesh();
}

const SharedRef<IMaterial>& AMeshActor::GetMaterial() const noexcept
{
    return mesh_.GetMaterial();
}
}
}
