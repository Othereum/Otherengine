#pragma once
#include "SceneComponent.hpp"
#include "Path.hpp"

namespace oeng
{
	class Shader;
	class Mesh;
	
	class OEAPI MeshComponent : public SceneComponent
	{
	public:
		explicit MeshComponent(AActor& owner, int update_order = 100);

		void Draw(Shader& shader) const noexcept;

		void SetMesh(Path file);
		void SetMesh(SharedPtr<Mesh> mesh) noexcept { mesh_ = std::move(mesh); }
		
	private:
		SharedPtr<Mesh> mesh_;
		size_t texture_idx_ = 0;
	};
}
