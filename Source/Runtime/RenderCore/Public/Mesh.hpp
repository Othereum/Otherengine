#pragma once
#include "VertexArray.hpp"

namespace oeng::renderer
{
class Material;
class Renderer;

class RENDERER_API Mesh : public ISerializable
{
public:
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

private:
    SharedPtr<Material> material_;
    VertexArray vertex_array_;
};
}
