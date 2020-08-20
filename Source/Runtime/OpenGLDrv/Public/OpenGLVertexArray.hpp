#pragma once
#include "VertexArray.hpp"

namespace oeng
{
inline namespace opengldrv
{
class OPENGLDRV_API OpenGLVertexArray : public VertexArray
{
public:
    DELETE_CPMV(OpenGLVertexArray);

    OpenGLVertexArray(std::vector<Vertex> vertices, std::vector<Vec3u16> indices);
    ~OpenGLVertexArray();

    void Activate() const noexcept override;

private:
    unsigned vertex_array_ = 0;
    unsigned vertex_buffer_ = 0;
    unsigned index_buffer_ = 0;
};
}
}
