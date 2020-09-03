#pragma once
#include "Actor.hpp"

namespace oeng
{
inline namespace engine
{
class Mesh;
class IMaterial;
class MeshComponent;

class ENGINE_API AMeshActor : public AActor
{
CLASS_BODY(AMeshActor)

public:
    AMeshActor();

    void SetMesh(std::shared_ptr<Mesh> mesh) const;
    void OverrideMaterial(std::shared_ptr<IMaterial> material) const;

    [[nodiscard]] const std::shared_ptr<Mesh>& GetMesh() const noexcept;
    [[nodiscard]] const std::shared_ptr<IMaterial>& GetMaterial() const noexcept;

    [[nodiscard]] MeshComponent& GetMeshComp() const noexcept
    {
        return mesh_;
    }

private:
    MeshComponent& mesh_;
};
}
}
