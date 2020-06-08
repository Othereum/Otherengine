#pragma once
#include "Path.hpp"
#include "Templates/DyArr.hpp"
#include "VertexArray.hpp"

namespace oeng
{
	class IEngine;
	class Texture;
	class VertexArray;
	
	class Mesh
	{
	public:
		Mesh(Path filepath, IEngine& engine);

		[[nodiscard]] auto& GetTextures() const noexcept { return textures_; }
		[[nodiscard]] const VertexArray& GetVertexArray() const noexcept { return vertex_array_; }
		[[nodiscard]] VertexArray& GetVertexArray() noexcept { return vertex_array_; }
		[[nodiscard]] Path GetShaderPath() const noexcept { return shader_path_; }
		[[nodiscard]] Float GetRadius() const noexcept { return radius_; }

		Mesh(const Mesh&) = delete;
		Mesh(Mesh&&) = delete;
		Mesh& operator=(const Mesh&) = delete;
		Mesh& operator=(Mesh&&) = delete;

	private:
		void LoadV1(const Json& json, IEngine& engine);
		
		DyArr<SharedPtr<Texture>> textures_;
		VertexArray vertex_array_;
		Path shader_path_;
		Float radius_;
	};
}
