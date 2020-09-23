#pragma once
#include "ShaderParam.hpp"

namespace logcat
{
extern ENGINE_API const LogCategory kMaterial;
}

namespace oeng
{
inline namespace rhi
{
class RHIShader;
}

inline namespace engine
{
class Texture;

class ENGINE_API IMaterial : public Object
{
  public:
    ~IMaterial();

    [[nodiscard]] static SharedRef<IMaterial> GetDefault();

    [[nodiscard]] auto& GetScalarParams() const noexcept
    {
        return scalars_;
    }

    [[nodiscard]] auto& GetVectorParams() const noexcept
    {
        return vectors_;
    }

    [[nodiscard]] auto& GetTextureParams() const noexcept
    {
        return textures_;
    }

    virtual void ApplyParams() const = 0;
    [[nodiscard]] virtual ScalarParam GetScalarParam(Name name) const;
    [[nodiscard]] virtual VectorParam GetVectorParam(Name name) const;
    [[nodiscard]] virtual SharedRef<Texture> GetTextureParam(Name name) const;
    [[nodiscard]] virtual RHIShader& GetRHI() const noexcept = 0;

  protected:
    void LoadParams(const Json& json);

    [[nodiscard]] virtual bool IsScalarParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsVectorParam(Name name) const = 0;
    [[nodiscard]] virtual bool IsTextureParam(Name name) const = 0;

    std::unordered_map<Name, ScalarParam> scalars_;
    std::unordered_map<Name, VectorParam> vectors_;
    std::unordered_map<Name, SharedRef<Texture>> textures_;

  private:
    template <class T>[[nodiscard]] bool IsValidParam(Name name) const;
    template <class T> void LoadParams(const Json& json, const std::string& key, std::unordered_map<Name, T>& out);
    template <class T, class Fn>
    void LoadParams(const Json& json, const std::string& key, std::unordered_map<Name, T>& out, Fn&& fn);
};
} // namespace engine
} // namespace oeng
