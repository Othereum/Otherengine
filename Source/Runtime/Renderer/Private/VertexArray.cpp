#include "VertexArray.hpp"
#include "Json.hpp"
#include "OpenGL.hpp"

RENDERER_BEGIN

void from_json(const Json& json, Vertex& vertex)
{
	json.get_to(vertex.arr);
	vertex.data.norm.Normalize();
}

VertexArray::VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices)
	:num_verts_{verts.size()}, num_indices_{indices.size()}
{
	gl(glGenVertexArrays, 1, &vertex_array_);
	Activate();

	gl(glGenBuffers, 1, &vertex_buffer_);
	gl(glBindBuffer, GL_ARRAY_BUFFER, vertex_buffer_);
	gl(glBufferData, GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);

	gl(glGenBuffers, 1, &index_buffer_);
	gl(glBindBuffer, GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
	gl(glBufferData, GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

	// for calculate offset
	constexpr Vertex* v = nullptr;
	constexpr auto size = static_cast<GLsizei>(sizeof(Vertex));

	static_assert(std::is_same_v<Float, float> || std::is_same_v<Float, double>);
	constexpr auto type = std::is_same_v<Float, float> ? GL_FLOAT : GL_DOUBLE;

	gl(glEnableVertexAttribArray, 0);
	gl(glVertexAttribPointer, 0, 3, type, false, size, &v->data.pos);

	gl(glEnableVertexAttribArray, 1);
	gl(glVertexAttribPointer, 1, 3, type, false, size, &v->data.norm);
	
	gl(glEnableVertexAttribArray, 2);
	gl(glVertexAttribPointer, 2, 2, type, false, size, &v->data.uv);
}

VertexArray::~VertexArray()
{
	GL(glDeleteBuffers, 1, &vertex_buffer_);
	GL(glDeleteBuffers, 1, &index_buffer_);
	GL(glDeleteVertexArrays, 1, &vertex_array_);
}

void VertexArray::Activate() const
{
	gl(glBindVertexArray, vertex_array_);
}

RENDERER_END
