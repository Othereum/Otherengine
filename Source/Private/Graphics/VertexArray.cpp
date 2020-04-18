#include "Graphics/VertexArray.h"
#include <GL/glew.h>

namespace oeng::graphics
{
	VertexArray::VertexArray(gsl::span<const Vec3> verts, gsl::span<const Vector<uint16_t, 3>> indices)
	{
		glGenVertexArrays(1, &vertex_array_);
		glBindVertexArray(vertex_array_);
		
		glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &index_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof Vec3, nullptr);
	}

	VertexArray::~VertexArray()
	{
		glDeleteBuffers(1, &vertex_buffer_);
		glDeleteBuffers(1, &index_buffer_);
		glDeleteVertexArrays(1, &vertex_array_);
	}
}