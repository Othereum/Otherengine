#include "actors/laser.h"
#include "actors/asteroid.h"
#include "components/sprite_component.h"
#include "components/circle_component.h"

namespace game
{
	laser::laser(application& app)
		:actor{app}
	{
		set_lifespan(1);

		auto& sprite = add_component<sprite_component>();
		sprite.set_texture("Asset/Laser.png");

		auto& col = add_component<circle_component>();
		col.set_radius(16);
		col.bind_on_overlap([this](circle_component& o)
		{
			if (auto a = dynamic_cast<asteroid*>(&o.get_owner()))
			{
				destroy();
				a->destroy();
			}
		});
	}
}
