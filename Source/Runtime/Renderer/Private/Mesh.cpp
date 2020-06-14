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
}
