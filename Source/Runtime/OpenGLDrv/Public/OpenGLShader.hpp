#pragma once
#include "RHIShader.hpp"

namespace oeng
{
inline namespace opengldrv
{
struct ShaderDeleter
{
    void operator()(unsigned id) const noexcept;
};

struct ProgramDeleter
{
    void operator()(unsigned id) const noexcept;
};

using ShaderHandle = Resource<unsigned, ShaderDeleter>;
using ProgramHandle = Resource<unsigned, ProgramDeleter>;

class OPENGLDRV_API OpenGLShader final : public RHIShader
{
  public:
    OpenGLShader(const char* vertex_shader, const char* frag_shader);

    void Activate() const noexcept override;

    bool ApplyParam(Name name, Float value) override;
    bool ApplyParam(Name name, const Vec4& value) override;
    bool ApplyParam(Name name, const Mat4& value) override;
    bool ApplyParam(Name name, RHITexture& value) override;

    [[nodiscard]] bool IsScalarParam(Name name) const override;
    [[nodiscard]] bool IsVectorParam(Name name) const override;
    [[nodiscard]] bool IsMatrixParam(Name name) const override;
    [[nodiscard]] bool IsTextureParam(Name name) const override;

  private:
    void LinkProgram();
    void Init();

    template <class T, class Fn>
    bool ApplyParam(Name name, const T& value, const std::unordered_map<Name, int>& loc_cache, Fn fn);

    ShaderHandle vertex_;
    ShaderHandle frag_;
    ProgramHandle program_;

    std::unordered_map<Name, int> scalar_loc_;
    std::unordered_map<Name, int> vector_loc_;
    std::unordered_map<Name, int> matrix_loc_;
    std::unordered_map<Name, int> texture_idx_;
};
} // namespace opengldrv
} // namespace oeng
