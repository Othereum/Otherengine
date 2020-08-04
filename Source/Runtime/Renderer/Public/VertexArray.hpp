#pragma once
#include <span>

namespace oeng::renderer
{
	struct RENDERER_API Vertex
	{
		struct Data
		{
			Vec3 pos;
			Vec3 norm;
			Vec2 uv;
		};

		union
		{
			Data data;
			Float arr[8];
		};

		constexpr Vertex() noexcept :data{} {}
		constexpr Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept
			:data{pos, norm, uv}
		{
		}
	};

	RENDERER_API void from_json(const Json& json, Vertex& vertex);

	class RENDERER_API VertexArray
	{
	public:
		VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices);
		void Load(std::span<const Vertex> verts, std::span<const Vec3u16> indices)
		{
			VertexArray{verts, indices}.swap(*this);
		}
		
		~VertexArray();
		
		VertexArray(VertexArray&&) = delete;
		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&&) = delete;
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
		size_t num_verts_;
		size_t num_indices_;
		unsigned vertex_buffer_{};
		unsigned index_buffer_{};
		unsigned vertex_array_{};
	};

	inline void swap(VertexArray& a, VertexArray& b) noexcept { a.swap(b); }
}
