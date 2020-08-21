#pragma once

namespace oeng
{
inline namespace rhi
{
struct Vertex
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
}
}
