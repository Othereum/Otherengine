#pragma once

namespace oeng::rhi
{
class Material;

struct RHI_API Vertex
{
    Vec3 pos;
    Vec3 norm;
    Vec2 uv;

    constexpr Vertex() noexcept = default;

    constexpr Vertex(const Vec3& pos, const Vec3& norm, const Vec2& uv) noexcept
        : pos{pos}, norm{norm}, uv{uv}
    {
    }

    [[nodiscard]] auto& AsArray() noexcept
    {
        return reinterpret_cast<Float(&)[8]>(*this);
    }

    [[nodiscard]] auto& AsArray() const noexcept
    {
        return reinterpret_cast<const Float(&)[8]>(*this);
    }
};

inline void from_json(const Json& json, Vertex& vertex)
{
    json.get_to(vertex.AsArray());
    vertex.norm.Normalize();
}

inline void to_json(Json& json, const Vertex& vertex)
{
    json = vertex.AsArray();
}

class RHI_API RHIMesh
{
public:
    DELETE_CPMV(RHIMesh);

    explicit RHIMesh(const fs::path& filepath);
    virtual ~RHIMesh() = default;

    virtual void Activate() const noexcept = 0;

    [[nodiscard]] auto& Vertices() const noexcept
    {
        return vertices_;
    }

    [[nodiscard]] auto& Indices() const noexcept
    {
        return indices_;
    }

    [[nodiscard]] Float Radius() const noexcept
    {
        return radius_;
    }

private:
    std::vector<Vertex> vertices_;
    std::vector<Vec3u16> indices_;
    Float radius_ = 0;
};
}
