#pragma once
#include "Asset.hpp"
#include "Templates/DyArr.hpp"
#include "VertexArray.hpp"

namespace oeng
{
	class Renderer;
	class Material;
	
	class OEAPI Mesh : public Asset
	{
	public:
		explicit Mesh(SharedPtr<Material> material) noexcept
			:material_{std::move(material)}, radius_{}
		{
		}
		
		/**
		 * \brief Load mesh from file
		 * \param path File path
		 * \param renderer Renderer
		 * \throw std::runtime_error If failed to open or parse
		 */
		Mesh(Path path, Renderer& renderer);
		void Load(Path path, Renderer& renderer)
		{
			Mesh{path, renderer}.swap(*this);
		}

		[[nodiscard]] Material& GetMaterial() const noexcept { return *material_; }
		[[nodiscard]] SharedPtr<Material> GetMaterialPtr() const noexcept { return material_; }
		[[nodiscard]] const VertexArray& GetVertexArray() const noexcept { return vertex_array_; }
		[[nodiscard]] VertexArray& GetVertexArray() noexcept { return vertex_array_; }
		[[nodiscard]] Float GetRadius() const noexcept { return radius_; }

		void swap(Mesh& r) noexcept
		{
			Asset::swap(r);
			using std::swap;
			swap(material_, r.material_);
			swap(vertex_array_, r.vertex_array_);
			swap(radius_, r.radius_);
		}

	private:
		SharedPtr<Material> material_;
		VertexArray vertex_array_;
		Float radius_;
	};

	inline void swap(Mesh& a, Mesh& b) noexcept { a.swap(b); }
}
