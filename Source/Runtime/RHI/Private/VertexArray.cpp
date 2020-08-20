#include "VertexArray.hpp"

namespace oeng::rhi
{
VertexArray::VertexArray(std::vector<Vertex> vertices, std::vector<Vec3u16> indices)
    : vertices_{std::move(vertices)}, indices_{std::move(indices)}
{
    CalcRadius();
}

void VertexArray::CalcRadius() noexcept
{
    auto max = 0_f;

    for (const auto& v : vertices_)
        max = Max(max, v.pos.DistSqr(Vec3::zero));

    radius_ = std::sqrt(max);
}
}
