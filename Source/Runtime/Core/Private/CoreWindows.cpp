#ifdef _WIN32

#include "Core.hpp"
#include <Windows.h>

namespace oeng
{
	bool IsDebugging() noexcept
	{
		return IsDebuggerPresent();
	}
}

#endif
