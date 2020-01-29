#pragma once

#include <NEG.h>

NEG_BEGIN

class component
{
public:
	explicit component(class actor& owner, int update_order = 100);
	virtual ~component() = default;

	component(const component&) = delete;
	component(component&&) = delete;
	component& operator=(const component&) = delete;
	component& operator=(component&&) = delete;
	
	virtual void update(float delta_seconds) {}
	[[nodiscard]] int get_update_order() const { return update_order_; }

	actor& owner;

private:
	int update_order_;
};

NEG_END