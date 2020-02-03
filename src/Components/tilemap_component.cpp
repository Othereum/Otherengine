#include "components/tilemap_component.h"

namespace game
{
	tilemap_component::tilemap_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
	}
}
