#pragma once
#include "move_component.h"

namespace game
{
	class pawn_move_comp : public move_component
	{
	public:
		explicit pawn_move_comp(actor& owner, int update_order = 10);

		void update(float delta_seconds) override;

		void add_input(fvector2 v) noexcept { input_ += v; }
		void add_rotation_input(degrees r) noexcept { rot_input_ += rot_input_; }

		void set_max_speed(float s) noexcept { max_speed_ = s; }
		void set_max_rotation_speed(degrees s) noexcept { max_rot_speed_ = s; }

	private:
		fvector2 input_;
		degrees rot_input_;
		
		float max_speed_ = 300;
		degrees max_rot_speed_ = 360_deg;
	};
}
