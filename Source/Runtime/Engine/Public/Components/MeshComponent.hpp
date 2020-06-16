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

		/**
		 * \brief Set mesh. If not found, set to engine's default.
		 * \param path Mesh path
		 */
		void SetMesh(Path path);
		
		/**
		 * \brief Set mesh
		 * \param mesh Pointer to mesh. Must not be nullptr
		 * \throw std::invalid_argument If mesh is nullptr
		 */
		void SetMesh(SharedPtr<Mesh> mesh);
		
		/**
		 * \brief Override default material of the mesh. If not found, set to engine's default.
		 * \param path Material path
		 */
		void SetMaterial(Path path);
		
		/**
		 * \brief Override default material of the mesh
		 * \param material Pointer to material. Must not be nullptr
		 * \throw std::invalid_argument If material is nullptr
		 */
		void SetMaterial(SharedPtr<Material> material);

		[[nodiscard]] Renderer& GetRenderer() const noexcept;

		MeshComponent(const MeshComponent&) = delete;
		MeshComponent(MeshComponent&&) = delete;
		MeshComponent& operator=(const MeshComponent&) = delete;
		MeshComponent& operator=(MeshComponent&&) = delete;
		
	private:
		void OnBeginPlay() override;
		void ReRegister() const;
		
		SharedPtr<Mesh> mesh_;
		SharedPtr<Material> material_;
	};
}
