#pragma once
#include "Serialization/Serializable.hpp"

namespace oeng
{
inline namespace engine
{
class ENGINE_API Material : public ISerializable
{
public:
    void TryUniforms();

    [[nodiscard]] Shader* GetShader() const noexcept
    {
        return shader_.get();
    }

    [[nodiscard]] Texture* GetTexture() const noexcept
    {
        return texture_.get();
    }

private:
    Material(Path path, Renderer& renderer, const Json& json);
    void LoadUniforms(const Json& uniforms);

    SharedPtr<Shader> shader_;
    SharedPtr<Texture> texture_;
    std::unordered_map<int, Uniform> uniforms_;
};
}
}
