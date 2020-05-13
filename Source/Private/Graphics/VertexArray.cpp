#include "VertexArray.hpp"
#include <GL/glew.h>
#include "Json.hpp"

namespace oeng
{
	void from_json(const Json& json, Vertex& vertex)
	{
		json.get_to(vertex.data);
	}

	VertexArray::VertexArray(gsl::span<const Vertex> verts, gsl::span<const Vec3u16> indices)
	{
		glGenVertexArrays(1, &vertex_array_);
		Activate();

		glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &index_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

		constexpr Vertex* v = nullptr;

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof Vertex, &v->pos);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof Vertex, &v->norm);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof Vertex, &v->uv);
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
