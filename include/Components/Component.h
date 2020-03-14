#pragma once

namespace game
{
	class actor;
	class application;
	
	class component
	{
	public:
		explicit component(actor& owner, int update_order = 100);
		virtual ~component() = default;

		component(const component&) = delete;
		component(component&&) = delete;
		component& operator=(const component&) = delete;
		component& operator=(component&&) = delete;
		
		virtual void update(float delta_seconds) {}
		[[nodiscard]] int get_update_order() const noexcept { return update_order_; }
		
		[[nodiscard]] actor& get_owner() const noexcept { return owner_; }
		[[nodiscard]] application& get_app() const noexcept;

	private:
		actor& owner_;
		int update_order_;
	};
}