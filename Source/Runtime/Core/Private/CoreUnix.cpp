#ifdef __unix__

#include "Core.hpp"
#include <sys/ptrace.h>

namespace oeng
{
	bool IsDebugging() noexcept
	{
		static const auto debugging = []
		{
			if (0 > ptrace(PTRACE_TRACEME, 0, 1, 0)) 
				return true;
			
			ptrace(PTRACE_DETACH, 0, 1, 0);
			return false;
		}();
		
		return debugging;
	}
}

#endif
