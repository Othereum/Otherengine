#pragma once
#include "Interfaces/Drawable.hpp"
#include "SceneComponent.hpp"
#include "Path.hpp"

namespace oeng
{
	class Shader;
	class Mesh;
	class Renderer;
	
	class OEAPI MeshComponent : public SceneComponent, public IMesh
	{
	public:
		explicit MeshComponent(AActor& owner, int update_order = 100);
		~MeshComponent();

		std::optional<DrawInfo> Draw() const noexcept override;
		Path GetShaderPath() const noexcept override;

		void SetMesh(Path file);
		void SetMesh(SharedPtr<Mesh> mesh) noexcept { mesh_ = std::move(mesh); }

		[[nodiscard]] Renderer& GetRenderer() const noexcept;

		MeshComponent(const MeshComponent&) = delete;
		MeshComponent(MeshComponent&&) = delete;
		MeshComponent& operator=(const MeshComponent&) = delete;
		MeshComponent& operator=(MeshComponent&&) = delete;
		
	private:
		void OnBeginPlay() override;
		
		SharedPtr<Mesh> mesh_;
		size_t texture_idx_ = 0;
	};
}
