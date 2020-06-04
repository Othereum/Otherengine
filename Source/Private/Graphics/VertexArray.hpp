#pragma once
#include <span>
#include "Math.hpp"
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
			Float data[8];
		};

		constexpr Vertex() noexcept :data{} {}
		constexpr Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept
			:pos{pos}, norm{norm}, uv{uv}
		{
		}
	};
	
	void from_json(const Json& json, Vertex& vertex);

	class VertexArray
	{
	public:
		VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices);
		~VertexArray();
		
		void Activate() const;

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
