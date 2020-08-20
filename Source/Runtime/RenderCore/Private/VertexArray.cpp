#include "VertexArray.hpp"
#include <GL/glew.h>

namespace oeng::renderer
{
void from_json(const Json& json, Vertex& vertex)
{
    json.get_to(vertex.AsArray());
    vertex.norm.Normalize();
}

void to_json(Json& json, const Vertex& vertex)
{
    json = vertex.AsArray();
}


VertexArray::VertexArray(std::vector<Vertex> vertices, std::vector<Vec3u16> indices)
    : vertices_{std::move(vertices)}, indices_{std::move(indices)}
{
    CalcRadius();
    Generate();
}

void VertexArray::Load(std::vector<Vertex> vertices, std::vector<Vec3u16> indices)
{
    VertexArray{std::move(vertices), std::move(indices)}.swap(*this);
}

VertexArray::VertexArray(const VertexArray& r)
    : vertices_{r.vertices_}, indices_{r.indices_}, radius_{r.radius_}
{
    if (r.IsValid())
        Generate();
}

VertexArray::VertexArray(VertexArray&& r) noexcept
    : vertices_{std::move(r.vertices_)}, indices_{std::move(r.indices_)}, radius_{r.radius_},
      vertex_array_{r.vertex_array_}, vertex_buffer_{r.vertex_buffer_}, index_buffer_{r.index_buffer_}
{
    radius_ = 0;
    vertex_array_ = 0;
    vertex_buffer_ = 0;
    index_buffer_ = 0;
}

VertexArray& VertexArray::operator=(const VertexArray& r)
{
    VertexArray{r}.swap(*this);
    return *this;
}

VertexArray& VertexArray::operator=(VertexArray&& r) noexcept
{
    VertexArray{std::move(r)}.swap(*this);
    return *this;
}

VertexArray::~VertexArray()
{
    // glDelete functions silently ignores 0
    glDeleteBuffers(1, &index_buffer_);
    glDeleteBuffers(1, &vertex_buffer_);
    glDeleteVertexArrays(1, &vertex_array_);
}

void VertexArray::Activate() const
{
    glBindVertexArray(vertex_array_);
}

void VertexArray::swap(VertexArray& r) noexcept
{
    using std::swap;
    swap(vertices_, r.vertices_);
    swap(indices_, r.indices_);
    swap(radius_, r.radius_);
    swap(vertex_buffer_, r.vertex_buffer_);
    swap(index_buffer_, r.index_buffer_);
    swap(vertex_array_, r.vertex_array_);
}

void VertexArray::Generate()
{
    assert(vertex_array_ == 0);

    glGenVertexArrays(1, &vertex_array_);
    Activate();

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), vertices_.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &index_buffer_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(Vec3u16), indices_.data(), GL_STATIC_DRAW);

    // for calculate offset
    constexpr const Vertex* v = nullptr;
    constexpr auto size = static_cast<int>(sizeof(Vertex));

    static_assert(std::is_same_v<Float, float> || std::is_same_v<Float, double>);
    constexpr auto type = std::is_same_v<Float, float> ? GL_FLOAT : GL_DOUBLE;

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, type, false, size, &v->pos);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, type, false, size, &v->norm);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, type, false, size, &v->uv);
}

void VertexArray::CalcRadius()
{
    auto max = 0_f;

    for (const auto& v : vertices_)
        max = Max(max, v.pos.DistSqr(Vec3::zero));

    radius_ = std::sqrt(max);
}

Archive& operator<<(Archive& archive, VertexArray& vertex_array)
{
    Json json;

    if (!archive.IsLoading())
        to_json(json, vertex_array);

    archive << json;

    if (archive.IsLoading())
        from_json(json, vertex_array);

    return archive;
}

void from_json(const Json& json, VertexArray& vertex_array)
{
    auto vertices = json.at("vertices").get<std::vector<Vertex>>();
    auto indices = json.at("indices").get<std::vector<Vec3u16>>();
    vertex_array.Load(std::move(vertices), std::move(indices));
}

void to_json(Json& json, const VertexArray& vertex_array)
{
    json["vertices"] = vertex_array.Vertices();
    json["indices"] = vertex_array.Indices();
}
}
