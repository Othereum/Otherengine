#pragma once

#include "neg.h"
#include <memory>
#include <vector>
#include "vector.h"

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
	[[nodiscard]] const vector2<>& get_pos() const { return pos_; }
	[[nodiscard]] float get_scale() const { return scale_; }
	[[nodiscard]] float get_rot() const { return rot_; }

	void update(float delta_seconds);

	void add_component(comp_ptr&& comp);
	void remove_component(const component& comp);

	void destroy();

	game& game;
	
private:
	void update_components(float delta_seconds);
	virtual void update_actor(float delta_seconds) {}

	state state_{};
	
	vector2<> pos_;
	float scale_{1};
	float rot_{};
	
	std::vector<comp_ptr> comps_;
};

NEG_END