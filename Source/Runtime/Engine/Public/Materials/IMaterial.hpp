#pragma once

namespace oeng
{
inline namespace rhi
{
class RHIShader;
}

inline namespace engine
{
class Texture;

class IMaterial : public Object
{
public:
    void LoadParams(const Json& json);

    [[nodiscard]] auto& GetScalarParams() const noexcept
    {
        return scalars_;
    }

    [[nodiscard]] auto& GetVectorParams() const noexcept
    {
        return scalars_;
    }

    [[nodiscard]] auto& GetTextureParams() const noexcept
    {
        return scalars_;
    }

    [[nodiscard]] virtual RHIShader& GetRHI() const noexcept = 0;

protected:
    [[nodiscard]] virtual bool IsScalarParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsVectorParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsTextureParam(Name name) const = 0;

    std::unordered_map<Name, Float> scalars_;
    std::unordered_map<Name, Vec4> vectors_;
    std::unordered_map<Name, std::shared_ptr<Texture>> textures_;

private:
    template <class T, class Fn>
    void LoadParams(const Json& json, std::unordered_map<Name, T>& out, Fn&& fn);

    template <class T>
    void LoadParams(const Json& json, std::unordered_map<Name, T>& out);

    template <class T>
    [[nodiscard]] bool IsValidParam(Name name) const;
};
}
}
