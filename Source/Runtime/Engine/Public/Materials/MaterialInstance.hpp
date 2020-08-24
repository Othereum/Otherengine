#pragma once
#include "ShaderParam.hpp"
#include "Materials/MaterialInterface.hpp"

namespace oeng
{
inline namespace engine
{
class Material;

class ENGINE_API MaterialInstance : public MaterialInterface
{
public:
    void Serialize(Archive& ar) override;

    void ApplyDefaultParams();
    void SetParam(Name name, const ShaderParam& param);

private:
    std::shared_ptr<Material> shader_;
};
}
}
