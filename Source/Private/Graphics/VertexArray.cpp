#include "Graphics/VertexArray.hpp"
#include <GL/glew.h>

namespace oeng
{
	VertexArray::VertexArray(gsl::span<const Vertex> verts, gsl::span<const Vec3u16> indices)
	{
		Construct(verts, indices);
	}

	VertexArray::VertexArray(VertexArray&& other) noexcept
	{
		*this = std::move(other);
	}

	VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
	{
		vertex_buffer_ = other.vertex_buffer_;
		index_buffer_ = other.index_buffer_;
		vertex_array_ = other.vertex_array_;
		
		other.vertex_buffer_ = 0;
		other.index_buffer_ = 0;
		other.vertex_array_ = 0;
		
		return *this;
	}

	void VertexArray::Construct(gsl::span<const Vertex> verts, gsl::span<const Vec3u16> indices)
	{
		if (!vertex_array_) glGenVertexArrays(1, &vertex_array_);
		Activate();

		if (!vertex_buffer_) glGenBuffers(1, &vertex_buffer_);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
		glBufferData(GL_ARRAY_BUFFER, verts.size_bytes(), verts.data(), GL_STATIC_DRAW);

		if (!index_buffer_) glGenBuffers(1, &index_buffer_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

		constexpr Vertex* v = nullptr;

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof Vertex, &v->pos);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof Vertex, &v->norm);
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof Vertex, &v->uv);
	}

	void VertexArray::Destruct() noexcept
	{
		auto del = [](unsigned& id, auto&& fn) { if (id) { fn(1, &id); id = 0; } };
		del(vertex_buffer_, glDeleteBuffers);
		del(index_buffer_, glDeleteBuffers);
		del(vertex_array_, glDeleteVertexArrays);
	}

	void VertexArray::Activate() const
	{
		glBindVertexArray(vertex_array_);
	}

	VertexArray::~VertexArray()
	{
		Destruct();
	}
}
