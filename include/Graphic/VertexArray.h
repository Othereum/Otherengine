#pragma once
#include <gsl/span>

namespace game
{
	class VertexArray
	{
	public:
		VertexArray(gsl::span<float> verts, gsl::span<unsigned> indices);
		~VertexArray();

		void Activate();

		[[nodiscard]] unsigned GetNumIndices() const noexcept { return num_indices_; }
		[[nodiscard]] unsigned GetNumVerts() const noexcept { return num_verts_; }

	private:
		unsigned num_verts_;
		unsigned num_indices_;
		unsigned vertex_buffer_;
		unsigned index_buffer_;
		unsigned vertex_array_;
	};
}
