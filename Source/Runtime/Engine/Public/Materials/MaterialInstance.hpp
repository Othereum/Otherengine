#pragma once
#include "Materials/MaterialInterface.hpp"

namespace oeng
{
inline namespace engine
{
class Material;

class ENGINE_API MaterialInstance : public MaterialInterface, public ISerializable
{
public:
    void Serialize(Archive& ar) override;

private:
    std::shared_ptr<MaterialInterface> parent_;
};
}
}
