#pragma once
#include "GameFramework/Actor.hpp"

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

    void SetMesh(SharedRef<Mesh> mesh) const;
    void OverrideMaterial(SharedRef<IMaterial> material) const;

    [[nodiscard]] const SharedRef<Mesh>& GetMesh() const noexcept;
    [[nodiscard]] const SharedRef<IMaterial>& GetMaterial() const noexcept;

    [[nodiscard]] MeshComponent& GetMeshComp() const noexcept
    {
        return mesh_;
    }

private:
    MeshComponent& mesh_;
};
}
}
