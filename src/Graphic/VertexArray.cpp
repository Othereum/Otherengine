#include "Graphic/VertexArray.h"
#include <GL/glew.h>

namespace game
{
	VertexArray::VertexArray(gsl::span<FVector3> verts, gsl::span<unsigned> indices)
	{
		glGenVertexArrays(1, &vertex_array_);
		glBindVertexArray(vertex_array_);
	}

	VertexArray::~VertexArray()
	{
	}

	void VertexArray::Activate()
	{
	}
}
