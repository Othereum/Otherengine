#pragma once

#include "NEG.h"
#include <memory>
#include <vector>
#include "Vector.h"

NEG_BEGIN

class actor
{
public:
	enum class state
	{
		active, paused, dead
	};

	actor(class game& game);
	virtual ~actor();

	actor(const actor&) = delete;
	actor(actor&&) = delete;
	actor& operator=(const actor&) = delete;
	actor& operator=(actor&&) = delete;

	void update(float delta_time);
	void update_components(float delta_time);
	virtual void update_actor(float delta_time);

	void add_component(std::unique_ptr<class component>&& component);
	void remove_component(const component& component);

private:
	state state_;
	
	vector2 position_;
	float scale_;
	float rotation_;
	
	std::vector<std::unique_ptr<component>> components_;
	game& game_;
};

NEG_END