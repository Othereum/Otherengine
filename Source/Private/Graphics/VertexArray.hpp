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

		Vertex() noexcept :data{} {}
		Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
			:pos{pos}, norm{norm}, uv{uv}
		{
		}
	};
	
	void from_json(const Json& json, Vertex& vertex);

	class VertexArray
	{
	public:
		constexpr VertexArray() noexcept = default;
		VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices);
		VertexArray(VertexArray&& r) noexcept;
		~VertexArray();
		
		VertexArray& operator=(VertexArray&& r) noexcept;

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void Activate() const;
		[[nodiscard]] size_t GetNumVerts() const noexcept { return num_verts_; }
		[[nodiscard]] size_t GetNumIndices() const noexcept { return num_indices_; }

	private:
		size_t num_verts_ = 0;
		size_t num_indices_ = 0;
		unsigned vertex_buffer_ = 0;
		unsigned index_buffer_ = 0;
		unsigned vertex_array_ = 0;
	};
}
