#include "Mesh.hpp"
#include <fstream>
#include "Engine.hpp"
#include "Json.hpp"
#include "VertexArray.hpp"

namespace oeng
{
	Mesh::Mesh(std::string_view filename, CEngine& engine)
	{
		std::ifstream file{filename.data()};
		Json json;
		file >> json;

		for (const auto& tex : json.at("textures"))
			textures_.push_back(engine.GetTexture(tex));

		const std::vector<Vertex> verts = json.at("vertices");
		const std::vector<Vec3u16> indices = json.at("indices");
		vertex_array_ = std::make_unique<VertexArray>(verts, indices);

		shader_name_ = json.at("shader");

		auto max = 0.f;
		for (const auto& v : verts) max = Max(max, v.pos.DistSqr({}));
		radius_ = sqrt(max);
	}

	Mesh::~Mesh() = default;
}
