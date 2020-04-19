#include "Components/TilemapComponent.h"
#include <fstream>
#include <sstream>
#include <string>
#include "Actor.h"
#include "Graphics/Renderer.h"

namespace oeng
{
	CTilemapComponent::CTilemapComponent(AActor& owner, int drawOrder, int updateOrder)
		:CSpriteComponent{owner, drawOrder, updateOrder}
	{
	}

	void CTilemapComponent::LoadTileFromCsv(const char* csvFilename)
	{
		tile_.clear();
		std::ifstream csv{csvFilename};
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

	void CTilemapComponent::Draw() const
	{
		for (size_t i = 0; i < tile_.size(); ++i)
		{
			for (size_t j = 0; j < tile_[i].size(); ++j)
			{
				const auto idx = tile_[i][j];
				if (idx == -1) continue;
				
				constexpr auto sz = 32, w = 8;
				const auto x = int(j), y = int(i);
				
				const SDL_Rect src{idx%w * sz, idx/w * sz, sz, sz};
				const SDL_Rect dest{x*sz, y*sz, sz, sz};
				
				GetRenderer().Draw(GetTexture(), src, dest);
			}
		}
	}
}
