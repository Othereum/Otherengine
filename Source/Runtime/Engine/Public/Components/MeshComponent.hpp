#pragma once
#include "Interfaces/Drawable.hpp"
#include "SceneComponent.hpp"
#include "Path.hpp"

namespace oeng::engine
{
	class ENGINE_API MeshComponent : public SceneComponent, public IMeshComponent
	{
	public:
		DELETE_CPMV(MeshComponent);
		
		explicit MeshComponent(AActor& owner, int update_order = 100);
		~MeshComponent();

		void SetMesh(Path path);
		void SetMesh(std::shared_ptr<Mesh> mesh);
		void SetMaterial(Path path);
		void SetMaterial(std::shared_ptr<Material> material);
		void SetMaxDrawDist(Float dist)	noexcept { max_draw_dist_ = dist; }
		
		[[nodiscard]] Float GetMaxDrawDist() const noexcept override { return max_draw_dist_; }
		[[nodiscard]] Float GetScaledRadius() const noexcept override { return radius_; }
		[[nodiscard]] Float GetUnscaledRadius() const noexcept override;
		[[nodiscard]] bool ShouldDraw() const noexcept override { return IsActive(); }
		[[nodiscard]] const Transform& GetDrawTrsf() const noexcept override { return GetWorldTrsf(); }
		[[nodiscard]] const Mat4& GetDrawTrsfMatrix() const noexcept override { return GetWorldTrsfMatrix(); }
		[[nodiscard]] Mesh& GetMesh() const noexcept override { return *mesh_; }
		[[nodiscard]] Material& GetMaterial() const noexcept override { return *material_; }

	protected:
		void OnBeginPlay() override;
		void OnTrsfChanged() override;

	private:
		void ReRegister() const;
		void RecalcRadius() noexcept;
		
		std::shared_ptr<Mesh> mesh_;
		std::shared_ptr<Material> material_;
		Float max_draw_dist_ = 10000;
		Float radius_ = 0;
	};
}
