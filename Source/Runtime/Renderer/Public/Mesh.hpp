#pragma once
#include "Path.hpp"
#include "Templates/DyArr.hpp"
#include "VertexArray.hpp"

namespace oeng
{
	class Renderer;
	class Material;
	
	class OEAPI Mesh
	{
	public:
		Mesh(Path path, Renderer& renderer);
		Mesh(Mesh&& r) noexcept;
		Mesh(const Mesh&) = delete;
		~Mesh() = default;

		Mesh& operator=(Mesh&& r) noexcept;
		Mesh& operator=(const Mesh&) = delete;

		[[nodiscard]] Material& GetMaterial() const noexcept { return *material_; }
		[[nodiscard]] SharedPtr<Material> GetMaterialPtr() const noexcept { return material_; }
		[[nodiscard]] const VertexArray& GetVertexArray() const noexcept { return vertex_array_; }
		[[nodiscard]] VertexArray& GetVertexArray() noexcept { return vertex_array_; }
		[[nodiscard]] Float GetRadius() const noexcept { return radius_; }

		void swap(Mesh& r) noexcept;

	private:		
		SharedPtr<Material> material_;
		VertexArray vertex_array_;
		Float radius_;
	};

	inline void swap(Mesh& a, Mesh& b) noexcept { a.swap(b); }
}
