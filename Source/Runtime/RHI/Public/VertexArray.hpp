#pragma once

namespace oeng
{
inline namespace rhi
{


class RHI_API VertexArray
{
public:
    DELETE_CPMV(VertexArray);

    VertexArray(std::vector<Vertex> vertices, std::vector<Vec3u16> indices);
    virtual ~VertexArray() = default;

    virtual void Activate() const noexcept = 0;


private:
    void CalcRadius() noexcept;

};
}
}
