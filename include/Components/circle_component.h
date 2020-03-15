#pragma once
#include "component.h"
#include <functional>

namespace game
{
	class circle_component : public component
	{
	public:
		explicit circle_component(actor& owner, int update_order = 100);
		~circle_component();

		void test_overlap(circle_component& other);
		void bind_on_overlap(std::function<void(circle_component&)>&& on_overlap) noexcept;

		void set_radius(float r) noexcept { radius_ = r; }
		[[nodiscard]] float get_radius() const noexcept { return radius_; }

	private:
		std::function<void(circle_component&)> on_overlap_;
		float radius_ = 32;
	};
}
