#pragma once
#include <gsl/span>
#include "Vector.h"

namespace game
{
	class VertexArray
	{
	public:
		VertexArray(gsl::span<FVector3> verts, gsl::span<unsigned> indices);
		~VertexArray();

		void Activate() const;

		[[nodiscard]] ptrdiff_t GetNumVerts() const noexcept { return num_verts_; }
		[[nodiscard]] ptrdiff_t GetNumIndices() const noexcept { return num_indices_; }

	private:
		ptrdiff_t num_verts_;
		ptrdiff_t num_indices_;
		unsigned vertex_buffer_;
		unsigned index_buffer_;
		unsigned vertex_array_;
	};
}
