#include "Mesh.hpp"
#include "Json.hpp"
#include "Renderer.hpp"

namespace oeng
{
	Mesh::Mesh(Path path, Renderer& renderer)
		:Mesh{path, renderer, ReadFileAsJson(path.CStr())}
	{
	}

	Mesh::Mesh(Path path, Renderer& renderer, const Json& json)
		:Mesh{path, renderer, json,
			json.at("vertices").get<DyArr<Vertex>>(),
			json.at("indices").get<DyArr<Vec3u16>>()}
	{
	}

	Mesh::Mesh(Path path, Renderer& renderer, const Json& json,
			std::span<const Vertex> verts, std::span<const Vec3u16> indices)
		:Asset{path},
		material_{renderer.GetMaterial(json.at("material"))},
		vertex_array_{verts, indices}
	{
		auto max = 0_f;
		for (const auto& v : verts) max = Max(max, v.data.pos.DistSqr(Vec3::zero));
		radius_ = std::sqrt(max);
	}
}
