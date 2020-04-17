#pragma once
#include <gsl/span>
#include "Vector.h"

namespace game::graphics
{
	class VertexArray
	{
	public:
		VertexArray(gsl::span<const Vec3> verts, gsl::span<const Vector<uint16_t, 3>> indices);
		~VertexArray();

	private:
		unsigned vertex_buffer_;
		unsigned index_buffer_;
		unsigned vertex_array_;
	};
}
