#include "Mesh.hpp"
#include "Format.hpp"
#include "Json.hpp"
#include "Renderer.hpp"

namespace oeng
{
	Mesh::Mesh(Path path, Renderer& renderer)
	{
		const auto json = ReadFileAsJson(path);
		
		material_ = renderer.GetMaterial(json.at("material"));

		const DyArr<Vertex> verts = json.at("vertices");
		const DyArr<Vec3u16> indices = json.at("indices");
		vertex_array_ = {verts, indices};

		auto max = 0_f;
		for (const auto& v : verts) max = Max(max, v.pos.DistSqr({}));
		radius_ = std::sqrt(max);
	}

	Mesh::Mesh(Mesh&& r) noexcept
		:material_{std::move(r.material_)},
		vertex_array_{std::move(r.vertex_array_)},
		radius_{r.radius_}
	{
		r.radius_ = 0;
	}

	Mesh& Mesh::operator=(Mesh&& r) noexcept
	{
		Mesh{std::move(r)}.swap(*this);
		return *this;
	}

	void Mesh::swap(Mesh& r) noexcept
	{
		using std::swap;
		swap(material_, r.material_);
		swap(vertex_array_, r.vertex_array_);
		swap(radius_, r.radius_);
	}
}
