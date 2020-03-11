#pragma once
#include "component.h"
#include "vector.h"

namespace game
{
	class move_component : public component
	{
	public:
		explicit move_component(actor& owner, int update_order = 10);

		void update(float delta_seconds) override;
		
		void set_velocity(fvector2 newvel) noexcept { vel_ = newvel; }
		void set_angular_velocity(degrees newvel) noexcept { rvel_ = newvel; }

		[[nodiscard]] fvector2 get_velocity() const noexcept { return vel_; }
		[[nodiscard]] degrees get_angular_velocity() const noexcept { return rvel_; }

	private:
		fvector2 vel_;
		degrees rvel_;
	};
}
