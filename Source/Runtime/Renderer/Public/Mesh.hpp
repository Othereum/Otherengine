#pragma once
#include "VertexArray.hpp"

namespace oeng::renderer
{
class Material;
class Renderer;

class RENDERER_API Mesh : public Asset
{
public:
    explicit Mesh(SharedRef<Material> material) noexcept
        : material_{std::move(material)}, vertex_array_{{}, {}}, radius_{}
    {
    }

    /**
     * Load mesh from file
     * @param path File path
     * @param renderer Renderer
     * @throw std::runtime_error If failed to open or parse
     */
    Mesh(Path path, Renderer& renderer);

    void Load(Path path, Renderer& renderer)
    {
        Mesh{path, renderer}.swap(*this);
    }

    [[nodiscard]] Material& GetMaterial() const noexcept
    {
        return *material_;
    }

    [[nodiscard]] SharedRef<Material> GetMaterialShared() const noexcept
    {
        return material_;
    }

    [[nodiscard]] const VertexArray& GetVertexArray() const noexcept
    {
        return vertex_array_;
    }

    [[nodiscard]] VertexArray& GetVertexArray() noexcept
    {
        return vertex_array_;
    }

    [[nodiscard]] Float GetRadius() const noexcept
    {
        return radius_;
    }

    void swap(Mesh& r) noexcept
    {
        Asset::swap(r);
        using std::swap;
        swap(material_, r.material_);
        swap(vertex_array_, r.vertex_array_);
        swap(radius_, r.radius_);
    }

private:
    Mesh(Path path, Renderer& renderer, const Json& json);
    Mesh(Path path, Renderer& renderer, const Json& json, std::span<const Vertex> verts,
         std::span<const Vec3u16> indices);

    SharedRef<Material> material_;
    VertexArray vertex_array_;
    Float radius_;
};

inline void swap(Mesh& a, Mesh& b) noexcept
{
    a.swap(b);
}
}
