#pragma once
#include "component.h"
#include <functional>
#include <unordered_set>

struct SDL_KeyboardEvent;

namespace game
{
	enum class key_event { released, pressed };
	using key_t = int;

	struct input_action
	{
		[[nodiscard]] virtual std::unordered_set<key_t> keys() const = 0;
	};

	struct axis_t
	{
		key_t key;
		float scale;
	};
	
	struct input_axis
	{
		[[nodiscard]] virtual std::vector<axis_t> keys() const = 0;
	};

	class input_component : public component
	{
	public:
		explicit input_component(class actor& owner, int update_order = 100, int input_receive_order = 100);
		~input_component();

		void begin_play() override;
		void process_input(const std::vector<int> (&events)[2], const uint8_t* keyboard) const;
		
		void bind_action(const input_action& action, key_event event, std::function<void()>&& callback);
		void bind_axis(const input_axis& axis, std::function<void(float)>&& callback);

		[[nodiscard]] int get_receive_order() const noexcept { return receive_order_; }
		
	private:
		std::vector<std::pair<std::unordered_set<key_t>, std::function<void()>>> actions_[2];
		std::vector<std::pair<std::vector<axis_t>, std::function<void(float)>>> axises_;

		int receive_order_;
	};
}
