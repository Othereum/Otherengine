#pragma once
#include "Actor.hpp"
#include "Path.hpp"

RENDERER_BEGIN
	class Renderer;
	class Mesh;
	class Material;
RENDERER_END


ENGINE_BEGIN

class MeshComponent;

class ENGINE_API AMeshActor : public AActor
{
public:
	explicit AMeshActor(World& world);
	void SetMesh(Path path) const;
	void SetMesh(SharedRef<Mesh> mesh) const;
	void SetMaterial(Path path) const;
	void SetMaterial(SharedRef<Material> material) const;
	[[nodiscard]] Mesh& GetMesh() const noexcept;
	[[nodiscard]] Material& GetMaterial() const noexcept;
	[[nodiscard]] MeshComponent& GetMeshComp() const noexcept { return mesh_; }

private:
	MeshComponent& mesh_;
};

ENGINE_END
