#pragma once
#include "Uniform.hpp"

namespace oeng::renderer
{
class Shader;
class Texture;
class Renderer;

class RENDERER_API Material : public Asset
{
public:
    /**
     * Load material from file
     * @param path File path
     * @param renderer Renderer
     * @throw std::runtime_error If failed to open or parse
     */
    Material(Path path, Renderer& renderer);

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
