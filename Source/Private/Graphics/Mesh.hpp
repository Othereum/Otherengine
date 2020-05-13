#pragma once
#include <memory>
#include <vector>
#include "Name.hpp"

namespace oeng
{
	class Engine;
	class Texture;
	class VertexArray;
	
	class Mesh
	{
	public:
		Mesh(std::string_view filename, Engine& engine);
		~Mesh();

		[[nodiscard]] auto& GetTextures() const noexcept { return textures_; }
		[[nodiscard]] VertexArray& GetVertexArray() const noexcept { return *vertex_array_; }
		[[nodiscard]] Name GetShaderName() const noexcept { return shader_name_; }
		[[nodiscard]] float GetRadius() const noexcept { return radius_; }

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) = delete;

	private:
		void LoadV1(const Json& json, Engine& engine);
		
		std::vector<std::shared_ptr<Texture>> textures_;
		std::unique_ptr<VertexArray> vertex_array_;
		Name shader_name_;
		float radius_;
	};
}
