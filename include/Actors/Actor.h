#pragma once

#include <memory>
#include <vector>
#include "vector.h"
#include "rotation.h"

namespace game
{
	class component;
	class application;
	
	class actor
	{
	public:
		enum class state
		{
			active, paused, dead
		};

		explicit actor(application& app);
		virtual ~actor();

		void begin_play() const;
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

		void set_pos(fvector2 new_pos) noexcept { pos_ = new_pos; }
		[[nodiscard]] fvector2 get_pos() const noexcept { return pos_; }

		void set_rot(degrees new_rot) noexcept { rot_ = new_rot; }
		[[nodiscard]] degrees get_rot() const noexcept { return rot_; }
		[[nodiscard]] fvector2 get_forward() const noexcept;

		[[nodiscard]] state get_state() const noexcept { return state_; }
		
		void set_scale(float scale) noexcept { scale_ = scale; }
		[[nodiscard]] float get_scale() const noexcept { return scale_; }

		[[nodiscard]] application& get_app() const noexcept { return app_; }

	private:
		void register_component(std::unique_ptr<component>&& comp);
		void update_components(float delta_seconds);
		virtual void update_actor(float delta_seconds) {}

		state state_{};
		
		fvector2 pos_;
		degrees rot_;
		float scale_{1};
		
		application& app_;
		std::vector<std::unique_ptr<component>> comps_;
	};
}