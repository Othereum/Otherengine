#pragma once
#include "SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
class Mesh;
class IMaterial;

class ENGINE_API MeshComponent : public SceneComponent
{
CLASS_BODY(MeshComponent)

public:
    MeshComponent();

    /**
     * Set mesh of this component.
     * @param mesh New mesh. Null NOT allowed.
     */
    void SetMesh(SharedRef<Mesh> mesh);

    /**
     * Override default material of mesh.
     * @param material New material. Null means un-override.
     */
    void OverrideMaterial(SharedRef<IMaterial> material);

    /**
     * Returns the scaled radius of a bound sphere.
     * @note Due to optimization, it's not accurate, but it's guaranteed that it's not less than the actual radius.
     */
    [[nodiscard]] Float GetScaledRadius() const noexcept;

    /**
     * Returns the unscaled radius of the bound sphere of the original mesh.
     */
    [[nodiscard]] Float GetUnscaledRadius() const noexcept;

    /**
     * @return Mesh. Guaranteed not null.
     */
    [[nodiscard]] const SharedRef<Mesh>& GetMesh() const noexcept
    {
        return mesh_;
    }

    /**
     * @return Material. Guaranteed not null. If overridden, returns it.
     */
    [[nodiscard]] const SharedRef<IMaterial>& GetMaterial() const noexcept;

    Float max_draw_dist = 10000_f;

protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;

private:
    void ReRegister();

    SharedRef<Mesh> mesh_;
    SharedRef<IMaterial> material_override_;
};
}
}
