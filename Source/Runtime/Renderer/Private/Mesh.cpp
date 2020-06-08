#include "Graphics/Mesh.hpp"
#include "Format.hpp"
#include "Engine.hpp"
#include "Json.hpp"

namespace oeng
{
	Mesh::Mesh(Path filepath, Engine& engine)
	{
		const auto json = ReadFileAsJson(filepath);

		const auto version = json.at("version").get<uintptr_t>();
		switch (version)
		{
		case 1:
			LoadV1(json, engine);
			break;

		default:
			throw std::runtime_error{format("Invalid version ({}) of mesh '{}'", version, filepath->string())};
		}
	}

	void Mesh::LoadV1(const Json& json, Engine& engine)
	{
		for (const auto& tex : json.at("textures"))
			textures_.push_back(engine.GetTexture(tex));

		const DyArr<Vertex> verts = json.at("vertices");
		const DyArr<Vec3u16> indices = json.at("indices");
		vertex_array_ = {verts, indices};

		shader_path_ = json.at("shader");

		auto max = 0_f;
		for (const auto& v : verts) max = Max(max, v.pos.DistSqr({}));
		radius_ = std::sqrt(max);
	}
}
