#pragma once

#include <NEG.h>
#include <memory>
#include <vector>
#include <Vector.h>

NEG_BEGIN

class actor
{
public:
	using comp_ptr = std::unique_ptr<class component>;
	
	enum class state
	{
		active, paused, dead
	};

	explicit actor(class game& game);
	virtual ~actor() = default;

	actor(const actor&) = delete;
	actor(actor&&) = delete;
	actor& operator=(const actor&) = delete;
	actor& operator=(actor&&) = delete;

	[[nodiscard]] state get_state() const { return state_; }

	void update(float delta_time);

	void add_component(comp_ptr&& comp);
	void remove_component(const component& comp);

	void destroy();

	game& game;
	
private:
	void update_components(float delta_time);
	virtual void update_actor(float delta_time) {}

	state state_{};
	
	vector2 pos_;
	float scale_{1};
	float rot_{};
	
	std::vector<comp_ptr> comps_;
};

NEG_END