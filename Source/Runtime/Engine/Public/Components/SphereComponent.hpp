#pragma once
#include "SceneComponent.hpp"

namespace oeng
{
inline namespace engine
{
/**
 * Simple sphere collision component.
 */
class ENGINE_API SphereComponent : public SceneComponent
{
CLASS_BODY(SphereComponent)

public:
    /**
     * Test if they overlap, and if so, broadcast the on_overlap event for both components.
     */
    void DoOverlap(SphereComponent& other);

    /**
     * Test whether both components overlap.
     * @return True if overlapped
     */
    [[nodiscard]] bool IsOverlap(const SphereComponent& other) const noexcept;

    /**
     * Add a function to be called when overlapping.
     */
    template <class Fn>
    void AddOnOverlap(Fn&& on_overlap)
    {
        on_overlap_.Add(std::forward<Fn>(on_overlap));
    }

    void SetUnscaledRadius(Float r) noexcept
    {
        radius_ = r;
    }

    /**
     * Returns the scaled radius of a bound sphere.
     * @note It does not support non-uniform scaling. It's scaled by largest scale component.
     */
    [[nodiscard]] Float GetScaledRadius() const noexcept
    {
        return radius_ * Max(GetWorldScale());
    }

    [[nodiscard]] Float GetUnscaledRadius() const noexcept
    {
        return radius_;
    }

protected:
    void OnBeginPlay() override;
    void OnEndPlay() override;

private:
    Event<SphereComponent&> on_overlap_;
    Float radius_ = 32;
};
}
}
