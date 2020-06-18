#include "VertexArray.hpp"
#include "OpenGL.hpp"
#include "Json.hpp"

namespace oeng
{
	void from_json(const Json& json, Vertex& vertex)
	{
		json.get_to(vertex.data);
		vertex.norm.Normalize();
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

		static_assert(std::is_same_v<Float, float> || std::is_same_v<Float, double>);
		constexpr auto type = std::is_same_v<Float, float> ? GL_FLOAT : GL_DOUBLE;

		gl(glEnableVertexAttribArray, 0);
		gl(glVertexAttribPointer, 0, 3, type, false, sizeof Vertex, &v->pos);

		gl(glEnableVertexAttribArray, 1);
		gl(glVertexAttribPointer, 1, 3, type, false, sizeof Vertex, &v->norm);
		
		gl(glEnableVertexAttribArray, 2);
		gl(glVertexAttribPointer, 2, 2, type, false, sizeof Vertex, &v->uv);
	}

	VertexArray::~VertexArray()
	{
		unsigned err;
		gl(err, glDeleteBuffers, 1, &vertex_buffer_);
		gl(err, glDeleteBuffers, 1, &index_buffer_);
		gl(err, glDeleteVertexArrays, 1, &vertex_array_);
	}

	void VertexArray::Activate() const
	{
		gl(glBindVertexArray, vertex_array_);
	}
}
