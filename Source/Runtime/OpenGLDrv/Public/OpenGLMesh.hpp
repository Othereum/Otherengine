#pragma once
#include "RHIMesh.hpp"
#include <span>

namespace oeng
{
inline namespace rhi
{
struct Vertex;
}

inline namespace opengldrv
{
class OPENGLDRV_API OpenGLMesh final : public RHIMesh
{
  public:
    DELETE_CPMV(OpenGLMesh);

    OpenGLMesh(std::span<const Vertex> vertices, std::span<const Vec3u16> indices);
    ~OpenGLMesh();

    void Activate() const noexcept override;
    void Draw() const noexcept override;

  private:
    unsigned vertex_array_ = 0;
    unsigned vertex_buffer_ = 0;
    unsigned index_buffer_ = 0;
};
} // namespace opengldrv
} // namespace oeng
