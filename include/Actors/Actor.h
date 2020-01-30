#pragma once

#include "neg.h"
#include <memory>
#include <vector>
#include "vector.h"
#include "rotation.h"

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

	void set_pos(const vector2<>& new_pos) { pos_ = new_pos; }
	[[nodiscard]] const vector2<>& get_pos() const { return pos_; }

	void set_rot(const degrees& new_rot) { rot_ = new_rot; }
	[[nodiscard]] const degrees& get_rot() const { return rot_; }

	[[nodiscard]] state get_state() const { return state_; }
	[[nodiscard]] float get_scale() const { return scale_; }

	void update(float delta_seconds);
	void destroy();

	template <class T, class... Args>
	T& add_component(Args&&... args)
	{
		static_assert(std::is_base_of_v<component, T>);
		auto ptr = std::make_unique<T>(*this, std::forward<Args>(args)...);
		auto& ref = *ptr;
		register_component(std::move(ptr));
		return ref;
	}

	game& game;
	
private:
	void register_component(comp_ptr&& comp);
	void update_components(float delta_seconds);
	virtual void update_actor(float delta_seconds) {}

	state state_{};
	
	vector2<> pos_;
	degrees rot_;
	float scale_{1};
	
	std::vector<comp_ptr> comps_;
};

NEG_END