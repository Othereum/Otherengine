#pragma once
#include <vector>
#include "SpriteComponent.h"

namespace oeng
{
	class CTilemapComponent : public CSpriteComponent
	{
	public:
		explicit CTilemapComponent(AActor& owner, int drawOrder = 10, int updateOrder = 100);
		void LoadTileFromCsv(const char* csvFilename);
		void Draw(const graphics::CRenderer& renderer) const override;

	private:
		std::vector<std::vector<int>> tile_;
	};
}
