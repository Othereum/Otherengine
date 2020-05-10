#pragma once
#include "Math.hpp"
#include <gsl/span>
#include "JsonFwd.hpp"

namespace oeng
{
	struct Vertex
	{
		union
		{
			struct
			{
				Vec3 pos;
				Vec3 norm;
				Vec2 uv;
			};
			float data[8];
		};

		constexpr Vertex() noexcept :data{} {}
		constexpr Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept
			:pos{pos}, norm{norm}, uv{uv}
		{
		}
	};
	
	class VertexArray
	{
	public:
		constexpr VertexArray() noexcept = default;
		VertexArray(gsl::span<const Vertex> verts, gsl::span<const Vec3u16> indices);
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&& other) noexcept;
		
		~VertexArray();
		
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& other) noexcept;

		void Construct(gsl::span<const Vertex> verts, gsl::span<const Vec3u16> indices);
		void Destruct() noexcept;

		void Activate() const;

	private:
		unsigned vertex_buffer_ = 0;
		unsigned index_buffer_ = 0;
		unsigned vertex_array_ = 0;
	};
}
