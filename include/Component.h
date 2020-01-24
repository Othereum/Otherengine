#pragma once

#include "NEG.h"

NEG_BEGIN

class component
{
public:
	component(class actor& owner, int update_order = 100);
	virtual ~component() = default;

	component(const component&) = delete;
	component(component&&) = delete;
	component& operator=(const component&) = delete;
	component& operator=(component&&) = delete;
	
	virtual void update(float delta_time) {}
	[[nodiscard]] int get_update_order() const { return update_order_; }

protected:
	actor& owner_;
	int update_order_;
};

NEG_END