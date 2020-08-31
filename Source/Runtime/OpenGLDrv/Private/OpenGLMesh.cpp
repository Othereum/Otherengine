#include "OpenGLMesh.hpp"
#include "Vertex.hpp"
#include <GL/glew.h>

namespace oeng
{
inline namespace opengldrv
{
OpenGLMesh::OpenGLMesh(std::span<const Vertex> vertices, std::span<const Vec3u16> indices)
{
    glGenVertexArrays(1, &vertex_array_);
    Activate();

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

    // for calculate offset
    constexpr const Vertex* v = nullptr;
    constexpr auto size = static_cast<int>(sizeof(Vertex));

    static_assert(std::is_same_v<Float, float> || std::is_same_v<Float, double>);
    constexpr auto type = std::is_same_v<Float, float> ? GL_FLOAT : GL_DOUBLE;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, type, false, size, &v->pos);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, type, false, size, &v->norm);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, type, false, size, &v->uv);
}

OpenGLMesh::~OpenGLMesh()
{
    // glDelete functions silently ignores 0
    glDeleteBuffers(1, &index_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteVertexArrays(1, &vertex_array_);
}

void OpenGLMesh::Activate() const noexcept
{
    glBindVertexArray(vertex_array_);
}
}
}
