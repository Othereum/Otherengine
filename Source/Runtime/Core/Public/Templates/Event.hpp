#pragma once
#include <functional>
#include "DyArr.hpp"

namespace oeng::core
{
	/**
	 * Delegates that can be bound to multiple functions and execute them all at once.
	 */
	template <class... Args>
	class Event
	{
	public:
		/**
		 * Add listener to this event.
		 * @param fn Callback function
		 * @return Listener lifecycle handler
		 */
		template <std::convertible_to<std::function<void(Args...)>> Fn>
		[[nodiscard]] void Add(Fn&& fn)
		{
			listeners_.emplace_back(std::forward<Fn>(fn));
		}

		/**
		 * Broadcasts this event to all bound listeners.
		 */
		void Broadcast(Args&&... args) const
		{
			for (auto& listener : listeners_)
			{
				listener(std::forward<Args>(args)...);
			}
		}

	private:
		DyArr<std::function<void(Args...)>> listeners_;
	};
}
