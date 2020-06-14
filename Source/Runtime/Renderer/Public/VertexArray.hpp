#pragma once
#include <span>
#include "Math.hpp"
#include "JsonFwd.hpp"

namespace oeng
{
	struct OEAPI Vertex
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
		constexpr Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept  // NOLINT(cppcoreguidelines-pro-type-member-init)
			:pos{pos}, norm{norm}, uv{uv}
		{
		}
	};
	
	OEAPI void from_json(const Json& json, Vertex& vertex);

	class OEAPI VertexArray
	{
	public:
		constexpr VertexArray() noexcept = default;
		
		VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices);
		void Load(std::span<const Vertex> verts, std::span<const Vec3u16> indices)
		{
			VertexArray{verts, indices}.swap(*this);
		}
		
		VertexArray(VertexArray&& r) noexcept
			:num_verts_{r.num_verts_}, num_indices_{r.num_indices_},
			vertex_buffer_{r.vertex_buffer_}, index_buffer_{r.index_buffer_}, vertex_array_{r.vertex_array_}
		{
			r.num_verts_ = 0;
			r.num_indices_ = 0;
			r.vertex_buffer_ = 0;
			r.index_buffer_ = 0;
			r.vertex_array_ = 0;
		}

		VertexArray& operator=(VertexArray&& r) noexcept
		{
			VertexArray{std::move(r)}.swap(*this);
			return *this;
		}
		
		~VertexArray();
		
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		void Activate() const;
		[[nodiscard]] size_t GetNumVerts() const noexcept { return num_verts_; }
		[[nodiscard]] size_t GetNumIndices() const noexcept { return num_indices_; }

		void swap(VertexArray& r) noexcept
		{
			using std::swap;
			swap(num_verts_, r.num_verts_);
			swap(num_indices_, r.num_indices_);
			swap(vertex_buffer_, r.vertex_buffer_);
			swap(index_buffer_, r.index_buffer_);
			swap(vertex_array_, r.vertex_array_);
		}

	private:
		size_t num_verts_ = 0;
		size_t num_indices_ = 0;
		unsigned vertex_buffer_ = 0;
		unsigned index_buffer_ = 0;
		unsigned vertex_array_ = 0;
	};

	inline void swap(VertexArray& a, VertexArray& b) noexcept { a.swap(b); }
}
