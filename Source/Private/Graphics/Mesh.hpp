#pragma once
#include "Path.hpp"
#include "Templates/DyArr.hpp"

namespace oeng
{
	class Engine;
	class Texture;
	class VertexArray;
	
	class Mesh
	{
	public:
		Mesh(Path filepath, Engine& engine);
		~Mesh();

		[[nodiscard]] auto& GetTextures() const noexcept { return textures_; }
		[[nodiscard]] VertexArray& GetVertexArray() const noexcept { return *vertex_array_; }
		[[nodiscard]] Path GetShaderPath() const noexcept { return shader_path_; }
		[[nodiscard]] float GetRadius() const noexcept { return radius_; }

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) = delete;

	private:
		void LoadV1(const Json& json, Engine& engine);
		
		DyArr<SharedPtr<Texture>> textures_;
		UniquePtr<VertexArray> vertex_array_;
		Path shader_path_;
		float radius_;
	};
}
