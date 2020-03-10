#pragma once
#include "component.h"
#include <map>
#include <functional>

namespace game
{
	enum class key_event { released, pressed };
	using key_t = int;

	struct input_action
	{
		[[nodiscard]] virtual std::pair<const key_t*, size_t> keys() const noexcept = 0;
	};

	struct axis_t
	{
		key_t key;
		float scale;
	};
	
	struct input_axis
	{
		[[nodiscard]] virtual std::pair<const axis_t*, size_t> keys() const noexcept = 0;
	};

	class input_component : public component
	{
	public:
		explicit input_component(class actor& owner, int update_order = 1);
		
		void bind_action(const input_action& action, key_event event, std::function<void()>&& callback);
		void bind_axis(const input_axis& axis, std::function<void()>&& callback);
		
	private:
		using fn_idx_t = size_t;
		
		std::multimap<key_t, fn_idx_t> actions_[2];
		std::vector<std::function<void()>> action_fns_;
		
		std::vector<std::pair<axis_t, fn_idx_t>> axises_;
		std::vector<std::function<void(float)>> axis_fns_;
	};
}
