#pragma once
#include "Actor.hpp"
#include "EngineFwd.hpp"
#include "Path.hpp"
#include "RendererFwd.hpp"

namespace oeng::engine
{
	class ENGINE_API AMeshActor : public AActor
	{
	public:
		explicit AMeshActor(World& world);
		void SetMesh(Path path) const;
		void SetMesh(SharedRef<Mesh> mesh) const;
		void SetMaterial(Path path) const;
		void SetMaterial(SharedRef<Material> material) const;
		void SetMaxDrawDist(Float dist) const noexcept;
		[[nodiscard]] Mesh& GetMesh() const noexcept;
		[[nodiscard]] Material& GetMaterial() const noexcept;
		[[nodiscard]] Float GetMaxDrawDist() const noexcept;
		[[nodiscard]] Float GetRadius() const noexcept;
		[[nodiscard]] MeshComponent& GetMeshComp() const noexcept { return mesh_; }

	private:
		MeshComponent& mesh_;
	};
}
