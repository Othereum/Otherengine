#pragma once
#include "Interfaces/Drawable.hpp"
#include "SceneComponent.hpp"
#include "Path.hpp"

namespace oeng
{
	class Mesh;
	class Material;
	class Renderer;
	
	class OEAPI MeshComponent : public SceneComponent, public IMeshComponent
	{
	public:
		explicit MeshComponent(AActor& owner, int update_order = 100);
		~MeshComponent();

		[[nodiscard]] bool ShouldDraw() const noexcept override { return IsEnabled(); }
		[[nodiscard]] const Mat4& GetDrawTrsf() const noexcept override { return GetWorldTrsfMatrix(); }
		[[nodiscard]] Mesh& GetMesh() const noexcept override { return *mesh_; }
		[[nodiscard]] Material& GetMaterial() const noexcept override { return *material_; }

		void SetMesh(Path path);
		void SetMesh(SharedPtr<Mesh> mesh) noexcept { mesh_ = std::move(mesh); }
		
		/**
		 * \brief Override default material of the mesh
		 * \param path Material path
		 */
		void SetMaterial(Path path);
		void SetMaterial(SharedPtr<Material> material) noexcept { material_ = std::move(material); }

		[[nodiscard]] Renderer& GetRenderer() const noexcept;

		MeshComponent(const MeshComponent&) = delete;
		MeshComponent(MeshComponent&&) = delete;
		MeshComponent& operator=(const MeshComponent&) = delete;
		MeshComponent& operator=(MeshComponent&&) = delete;
		
	private:
		void OnBeginPlay() override;
		
		SharedPtr<Mesh> mesh_;
		SharedPtr<Material> material_;
	};
}
