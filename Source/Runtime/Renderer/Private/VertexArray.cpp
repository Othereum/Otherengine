#include "VertexArray.hpp"
#include <GL/glew.h>
#include "Json.hpp"

namespace oeng
{
	void from_json(const Json& json, Vertex& vertex)
	{
		json.get_to(vertex.data);
	}

	VertexArray::VertexArray(std::span<const Vertex> verts, std::span<const Vec3u16> indices)
		:num_verts_{verts.size()}, num_indices_{indices.size()}
	{
		glGenVertexArrays(1, &vertex_array_);
		Activate();

		glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);

		glGenBuffers(1, &index_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

		// for calculate offset
		constexpr Vertex* v = nullptr;

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof Vertex, &v->pos);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof Vertex, &v->norm);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof Vertex, &v->uv);
	}

	VertexArray::VertexArray(VertexArray&& r) noexcept
		:num_verts_{r.num_verts_}, num_indices_{r.num_indices_},
		vertex_buffer_{r.vertex_buffer_}, index_buffer_{r.index_buffer_}, vertex_array_{r.vertex_array_}
	{
		r.num_verts_ = 0;
		r.num_indices_ = 0;
		r.vertex_buffer_ = 0;
		r.index_buffer_ = 0;
		r.vertex_array_ = 0;
	}

	VertexArray::~VertexArray()
	{
		// glDelete functions silently ignores 0 or invalid ID.
		glDeleteBuffers(1, &vertex_buffer_);
		glDeleteBuffers(1, &index_buffer_);
		glDeleteVertexArrays(1, &vertex_array_);
	}

	VertexArray& VertexArray::operator=(VertexArray&& r) noexcept
	{
		this->~VertexArray();
		new (this) VertexArray{std::move(r)};
		return *this;
	}

	void VertexArray::Activate() const
	{
		glBindVertexArray(vertex_array_);
	}
}
