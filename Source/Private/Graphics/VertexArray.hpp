#pragma once
#include "Math.hpp"

namespace oeng
{
	struct Vertex
	{
		Vec3 pos;
		Vec3 norm;
		Vec2 uv;

		constexpr Vertex() noexcept = default;
		constexpr Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept
			:pos{pos}, norm{norm}, uv{uv}
		{
		}
	};
	
	class VertexArray
	{
	public:
		template <size_t NumVerts, size_t NumIndices>
		VertexArray(const Vertex (&verts)[NumVerts], const Vec3u16 (&indices)[NumIndices])
			:VertexArray{verts, NumVerts, indices, NumIndices}
		{
		}
		
		VertexArray(const Vertex* verts, size_t num_verts, const Vec3u16* indices, size_t num_indices);
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