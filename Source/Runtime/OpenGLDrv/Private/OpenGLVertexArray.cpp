#include "OpenGLVertexArray.hpp"
#include <GL/glew.h>

namespace oeng
{
inline namespace opengldrv
{
OpenGLVertexArray::OpenGLVertexArray(std::vector<Vertex> vertices, std::vector<Vec3u16> indices)
    : VertexArray{std::move(vertices), std::move(indices)}
{
    glGenVertexArrays(1, &vertex_array_);
    OpenGLVertexArray::Activate();

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, Vertices().size() * sizeof(Vertex), Vertices().data(), GL_STATIC_DRAW);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices().size() * sizeof(Vec3u16), Indices().data(), GL_STATIC_DRAW);

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

OpenGLVertexArray::~OpenGLVertexArray()
{
    // glDelete functions silently ignores 0
    glDeleteBuffers(1, &index_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteVertexArrays(1, &vertex_array_);
}

void OpenGLVertexArray::Activate() const noexcept
{
    glBindVertexArray(vertex_array_);
}
}
}
