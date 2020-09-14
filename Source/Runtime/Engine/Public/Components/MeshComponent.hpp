#pragma once
#include "SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class Mesh;
class IMaterial;
class IRenderer;

class ENGINE_API MeshComponent : public SceneComponent
{
    CLASS_BODY(MeshComponent)

  public:
    MeshComponent();

    /**
     * Set mesh of this component.
     * @param mesh New mesh
     */
    void SetMesh(SharedRef<Mesh> mesh);

    /**
     * Override default material of mesh.
     * @param material New material. Null means un-override.
     */
    void OverrideMaterial(SharedPtr<IMaterial> material);

    /**
     * Returns the scaled radius of a bound sphere.
     * @note Due to optimization, it's not accurate, but it's guaranteed that it's not less than the actual radius.
     */
    [[nodiscard]] Float GetScaledRadius() const noexcept;

    /**
     * Returns the unscaled radius of the bound sphere of the original mesh.
     */
    [[nodiscard]] Float GetUnscaledRadius() const noexcept;

    [[nodiscard]] SharedRef<Mesh> GetMesh() const noexcept
    {
        return mesh_;
    }

    [[nodiscard]] SharedRef<IMaterial> GetMaterial() const noexcept;
    [[nodiscard]] IRenderer& GetRenderer() const noexcept;

    Float max_draw_dist = 10000_f;

  protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;

  private:
    void ReRegister();

    SharedRef<Mesh> mesh_;
    SharedPtr<IMaterial> material_override_;
};
} // namespace engine
} // namespace oeng
