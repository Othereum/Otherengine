#pragma once

namespace game
{
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
		[[nodiscard]] actor& get_owner() const { return owner_; }

	private:
		actor& owner_;
		int update_order_;
	};
}