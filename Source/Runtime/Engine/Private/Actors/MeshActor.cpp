#include "Actors/MeshActor.hpp"
#include "Components/MeshComponent.hpp"

namespace oeng::engine
{
	AMeshActor::AMeshActor(World& world)
		:AActor{world}, mesh_{AddComponent<MeshComponent>()}
	{
		SetRootComponent(&mesh_);
	}

	void AMeshActor::SetMesh(Path path) const
	{
		mesh_.SetMesh(path);
	}

	void AMeshActor::SetMesh(SharedRef<Mesh> mesh) const
	{
		mesh_.SetMesh(std::move(mesh));
	}

	void AMeshActor::SetMaterial(Path path) const
	{
		mesh_.SetMaterial(path);
	}

	void AMeshActor::SetMaterial(SharedRef<Material> material) const
	{
		mesh_.SetMaterial(std::move(material));
	}

	Mesh& AMeshActor::GetMesh() const noexcept
	{
		return mesh_.GetMesh();
	}

	Material& AMeshActor::GetMaterial() const noexcept
	{
		return mesh_.GetMaterial();
	}
}
