#include "components/tilemap_component.h"
#include <fstream>
#include <sstream>
#include <string>
#include "renderer.h"
#include "actors/actor.h"

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

	void tilemap_component::draw(renderer& renderer) const
	{
		for (size_t i = 0; i < tile_.size(); ++i)
		{
			for (size_t j = 0; j < tile_[i].size(); ++j)
			{
				const auto idx = tile_[i][j];
				if (idx == -1) continue;
				
				constexpr auto sz = 32, w = 8;
				const auto x = static_cast<int>(j), y = static_cast<int>(i);
				
				const SDL_Rect src{idx%w * sz, idx/w * sz, sz, sz};
				const SDL_Rect dest{x*sz, y*sz, sz, sz};
				
				renderer.draw(get_texture(), src, dest);
			}
		}
	}
}
