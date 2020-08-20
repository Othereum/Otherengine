#pragma once

namespace oeng::renderer
{
struct RENDERER_API Vertex
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

RENDERER_API void from_json(const Json& json, Vertex& vertex);
RENDERER_API void to_json(Json& json, const Vertex& vertex);


class RENDERER_API VertexArray
{
public:
    VertexArray() = default;
    ~VertexArray();

    VertexArray(std::vector<Vertex> vertices, std::vector<Vec3u16> indices);
    void Load(std::vector<Vertex> vertices, std::vector<Vec3u16> indices);

    VertexArray(const VertexArray& r);
    VertexArray(VertexArray&& r) noexcept;
    VertexArray& operator=(const VertexArray& r);
    VertexArray& operator=(VertexArray&& r) noexcept;

    void Activate() const;

    [[nodiscard]] bool IsValid() const noexcept
    {
        return vertex_array_ != 0;
    }

    [[nodiscard]] auto& Vertices() const noexcept
    {
        return vertices_;
    }

    [[nodiscard]] auto& Indices() const noexcept
    {
        return indices_;
    }

    void swap(VertexArray& r) noexcept;

    friend RENDERER_API Archive& operator<<(Archive& archive, VertexArray& vertex_array);

private:
    void Generate();
    void CalcRadius();

    std::vector<Vertex> vertices_;
    std::vector<Vec3u16> indices_;
    Float radius_ = 0;
    unsigned vertex_array_ = 0;
    unsigned vertex_buffer_ = 0;
    unsigned index_buffer_ = 0;
};

inline void swap(VertexArray& a, VertexArray& b) noexcept
{
    a.swap(b);
}

RENDERER_API void from_json(const Json& json, VertexArray& vertex_array);
RENDERER_API void to_json(Json& json, const VertexArray& vertex_array);
}
