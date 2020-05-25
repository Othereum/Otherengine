#include "Mesh.hpp"
#include <fstream>
#include <fmt/core.h>
#include "Engine.hpp"
#include "Json.hpp"
#include "VertexArray.hpp"

namespace oeng
{
	Mesh::Mesh(Path filepath, Engine& engine)
	{
		std::ifstream file{filepath};
		Json json;
		file >> json;

		const auto version = json.at("version").get<int>();
		switch (version)
		{
		case 1:
			LoadV1(json, engine);
			break;

		default:
			throw std::runtime_error{fmt::format("Invalid version ({}) of mesh '{}'", version, filepath->string())};
		}
	}

	Mesh::~Mesh() = default;

	void Mesh::LoadV1(const Json& json, Engine& engine)
	{
		for (const auto& tex : json.at("textures"))
			textures_.push_back(engine.GetTexture(tex));

		const std::vector<Vertex> verts = json.at("vertices");
		const std::vector<Vec3u16> indices = json.at("indices");
		vertex_array_ = std::make_unique<VertexArray>(verts, indices);

		shader_path_ = json.at("shader");

		auto max = 0.f;
		for (const auto& v : verts) max = Max(max, v.pos.DistSqr({}));
		radius_ = sqrt(max);
	}
}
