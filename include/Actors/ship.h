#pragma once

#include "actor.h"

namespace game
{
	class ship : public actor
	{
	public:
		explicit ship(world& outer);
		void update_actor(float delta_seconds) override;
		void process_keyboard(const uint8_t* state);
		[[nodiscard]] fvector2 get_right_speed() const { return velocity_; }

	private:
		class anim_sprite_component& anim_;
		fvector2 velocity_;
	};
}
