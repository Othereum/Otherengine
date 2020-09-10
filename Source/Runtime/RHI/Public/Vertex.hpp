#pragma once

namespace oeng
{
inline namespace rhi
{
struct Vertex
{
    Vec3 pos;
    UVec3 norm = UVec3::Up();
    Vec2 uv;

    constexpr Vertex() noexcept = default;

    constexpr Vertex(const Vec3& pos, const UVec3& norm, const Vec2& uv) noexcept : pos{pos}, norm{norm}, uv{uv}
    {
    }

    [[nodiscard]] auto& AsArray() const noexcept
    {
        return reinterpret_cast<const Float(&)[8]>(*this);
    }
};

inline void from_json(const Json& json, Vertex& vertex)
{
    vertex.pos[0] = json[0].get<Float>();
    vertex.pos[1] = json[1].get<Float>();
    vertex.pos[2] = json[2].get<Float>();

    Vec3 norm{json[3].get<Float>(), json[4].get<Float>(), json[5].get<Float>()};
    vertex.norm = norm.Unit().value();

    vertex.uv[0] = json[6].get<Float>();
    vertex.uv[1] = json[7].get<Float>();
}

inline void to_json(Json& json, const Vertex& vertex)
{
    json = vertex.AsArray();
}
} // namespace rhi
} // namespace oeng
