#pragma once
#include "Math.hpp"

namespace oeng::graphics
{
	struct Vertex
	{
		Vec3 pos;
		Vec2 uv;

		constexpr Vertex() noexcept = default;
		constexpr Vertex(Vec3 pos, Vec2 uv) noexcept: pos{pos}, uv{uv} {}
	};
	
	class VertexArray
	{
	public:
		template <size_t NumVerts, size_t NumIndices>
		VertexArray(const Vertex (&verts)[NumVerts], const Vector<uint16_t, 3> (&indices)[NumIndices])
			:VertexArray{verts, NumVerts, indices, NumIndices}
		{
		}
		
		VertexArray(const Vertex* verts, size_t num_verts, const Vector<uint16_t, 3>* indices, size_t num_indices);
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&&) = delete;

	private:
		unsigned vertex_buffer_ = 0;
		unsigned index_buffer_ = 0;
		unsigned vertex_array_ = 0;
	};
}
