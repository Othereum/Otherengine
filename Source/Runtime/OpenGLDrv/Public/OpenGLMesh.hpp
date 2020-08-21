#pragma once
#include "RHIResource.hpp"
#include <span>

namespace oeng
{
namespace rhi
{
struct Vertex;
}
}

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLMesh : public RHIResource
{
public:
    DELETE_CPMV(OpenGLMesh);

    OpenGLMesh(std::span<const Vertex> vertices, std::span<const Vec3u16> indices);
    ~OpenGLMesh();

    void Activate() const noexcept override;

private:
    unsigned vertex_array_ = 0;
    unsigned vertex_buffer_ = 0;
    unsigned index_buffer_ = 0;
};
}
}
