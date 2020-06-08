#pragma once
#include "API.hpp"

namespace oeng
{
	/**
	 * \brief Check if current context is in game(main) thread
	 */
	OEAPI bool IsGameThread() noexcept;

	OEAPI bool IsEngineExists() noexcept;
}
