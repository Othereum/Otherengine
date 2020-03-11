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
		
		void add_movement_input(fvector2 v);
		void add_rotation_input(degrees r);

	private:
		fvector2 movement_;
		degrees rotation_;
	};
}
