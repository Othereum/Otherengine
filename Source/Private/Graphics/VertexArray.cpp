#include "Graphics/VertexArray.h"
#include <GL/glew.h>

namespace oeng::graphics
{
	VertexArray::VertexArray(const Vertex* verts, size_t num_verts, const Vector<uint16_t, 3>* indices, size_t num_indices)
	{
		glGenVertexArrays(1, &vertex_array_);
		glBindVertexArray(vertex_array_);
		
		glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, sizeof Vertex * num_verts, verts, GL_STATIC_DRAW);

		glGenBuffers(1, &index_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof Vector<uint16_t, 3> * num_indices, indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof Vertex, nullptr);

		constexpr auto offset = reinterpret_cast<const void*>(offsetof(Vertex, uv));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof Vertex, offset);
	}

	VertexArray::~VertexArray()
	{
		glDeleteBuffers(1, &vertex_buffer_);
		glDeleteBuffers(1, &index_buffer_);
		glDeleteVertexArrays(1, &vertex_array_);
	}
}
