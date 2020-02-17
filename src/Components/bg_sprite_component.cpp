#include "components/bg_sprite_component.h"

namespace game
{
	bg_sprite_component::bg_sprite_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
	}
}
