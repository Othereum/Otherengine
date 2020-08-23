#include "Engine/Mesh.hpp"
#include "AssetManager.hpp"
#include "DynamicRHI.hpp"
#include "RHIResource.hpp"
#include "Vertex.hpp"

namespace oeng
{
inline namespace engine
{
void Mesh::Serialize(Archive& ar)
{
    const auto json = ar.ReadAllAsJson();

    const auto vertices = json.at("vertices").get<std::vector<Vertex>>();
    const auto indices = json.at("indices").get<std::vector<Vec3u16>>();

    material_ = AssetManager::Get().Load<Material>(json.at("material").get<Path>());
    rhi_.reset(DynamicRHI::Get().CreateMesh(vertices, indices));

    auto max = 0_f;
    for (const auto& v : vertices)
        max = Max(max, v.pos.DistSqr(Vec3::zero));
    radius_ = std::sqrt(max);
}

void Mesh::Activate() const noexcept
{
    rhi_->Activate();
}
}
}
