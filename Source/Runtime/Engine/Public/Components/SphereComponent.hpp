#pragma once
#include "SceneComponent.hpp"
#include "Templates/Event.hpp"

ENGINE_BEGIN

/**
 * Simple sphere collision component.
 */
class ENGINE_API SphereComponent : public SceneComponent
{
public:
	DELETE_CPMV(SphereComponent);
	
	explicit SphereComponent(AActor& owner, int update_order = 100);
	~SphereComponent();

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

	void SetUnscaledRadius(Float r) noexcept { radius_ = r; }
	[[nodiscard]] Float GetScaledRadius() const noexcept { return radius_ * Max(GetWorldScale()); }
	[[nodiscard]] Float GetUnscaledRadius() const noexcept { return radius_; }

protected:
	void OnBeginPlay() override;

private:
	Event<SphereComponent&> on_overlap_;
	Float radius_ = 32;
};

ENGINE_END
