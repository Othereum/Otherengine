#include "VertexArray.hpp"
#include <GL/glew.h>
#include "Json.hpp"

namespace oeng::renderer
{
	void from_json(const Json& json, Vertex& vertex)
	{
		json.get_to(vertex.arr);
		vertex.data.norm.Normalize();
	}

	VertexArray::VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices)
		:num_verts_{verts.size()}, num_indices_{indices.size()}
	{
		glGenVertexArrays(1, &vertex_array_);
		Activate();

		glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &index_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

		// for calculate offset
		constexpr Vertex* v = nullptr;
		constexpr auto size = static_cast<GLsizei>(sizeof(Vertex));

		static_assert(std::is_same_v<Float, float> || std::is_same_v<Float, double>);
		constexpr auto type = std::is_same_v<Float, float> ? GL_FLOAT : GL_DOUBLE;

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, type, false, size, &v->data.pos);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, type, false, size, &v->data.norm);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, type, false, size, &v->data.uv);
	}

	VertexArray::~VertexArray()
	{
		glDeleteBuffers(1, &vertex_buffer_);
		glDeleteBuffers(1, &index_buffer_);
		glDeleteVertexArrays(1, &vertex_array_);
	}

	void VertexArray::Activate() const
	{
		glBindVertexArray(vertex_array_);
	}
}
