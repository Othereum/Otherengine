#pragma once
#include "API.hpp"

namespace oeng
{
	/**
	 * \brief Check if current context is in game(main) thread
	 */
	[[nodiscard]] OEAPI bool IsGameThread() noexcept;
}
