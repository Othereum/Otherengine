#include "Graphic/VertexArray.h"
#include <GL/glew.h>

namespace game
{
	VertexArray::VertexArray(gsl::span<FVector3> verts, gsl::span<unsigned> indices)
	{
		glGenVertexArrays(1, &vertex_array_);
		glBindVertexArray(vertex_array_);
		glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);
	}

	VertexArray::~VertexArray()
	{
	}

	void VertexArray::Activate()
	{
	}
}
