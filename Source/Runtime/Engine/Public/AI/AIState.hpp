#pragma once
#include "Name.hpp"

namespace oeng
{
inline namespace engine
{
namespace ai_state
{
class Base
{
public:
    DELETE_CPMV(Base);

    explicit Base(class AIComponent& owner) noexcept
        : owner_{owner}
    {
    }

    explicit Base(class AIComponent* owner) noexcept
        : owner_{*owner}
    {
    }

    virtual ~Base() = default;

    virtual void OnEnter([[maybe_unused]] Base& prev)
    {
    }

    virtual void Update([[maybe_unused]] Float delta_seconds)
    {
    }

    virtual void OnExit([[maybe_unused]] Base& next)
    {
    }

    [[nodiscard]] virtual Name GetName() const = 0;

    [[nodiscard]] AIComponent& GetOwner() const noexcept
    {
        return owner_;
    }

private:
    AIComponent& owner_;
};
}
}
}
