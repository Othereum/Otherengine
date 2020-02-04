#include "components/tilemap_component.h"
#include <fstream>
#include <sstream>
#include <string>

namespace game
{
	tilemap_component::tilemap_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
	}

	void tilemap_component::load_tile_from_csv(const char* csv_filename)
	{
		tile_.clear();
		std::ifstream csv{csv_filename};
		for (std::string line; std::getline(csv, line);)
		{
			std::vector<int> row;
			std::stringstream ss{line};
			for (std::string cell; std::getline(ss, cell, ',');)
			{
				row.push_back(std::stoi(cell));
			}
			tile_.push_back(std::move(row));
		}
	}
}
