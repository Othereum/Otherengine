#pragma once
#include "sprite_component.h"
#include <vector>

namespace game
{
	class tilemap_component : public sprite_component
	{
	public:
		explicit tilemap_component(actor& owner, int draw_order = 10, int update_order = 100);
		void load_tile_from_csv(const char* csv_filename);
		void draw() const override;

	private:
		std::vector<std::vector<int>> tile_;
	};
}
