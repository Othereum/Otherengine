#ifdef __unix__

#include <cpuid.h>
#include <dlfcn.h>
#include <stdexcept>
#include <sys/ptrace.h>
#include "Platform.hpp"

namespace oeng::plf
{
	bool IsDebugging() noexcept
	{
		static const auto is_debugger_present = []
		{
			if (0 > ptrace(PTRACE_TRACEME, 0, 1, 0)) 
				return true;
			
			ptrace(PTRACE_DETACH, 0, 1, 0);
			return false;
		}();
		
		return is_debugger_present;
	}
	
	Dll::Dll(const char* filepath)
	{
		const auto dll = dlopen(filepath, RTLD_LAZY);
		if (!dll) throw std::runtime_error{dlerror()};

		dll_.reset(dll, &dlclose);
		filepath_ = filepath;
	}

	void* Dll::GetSymbol(const char* name) const
	{
		auto* const sym = FindSymbol(name);
		if (!sym) throw std::runtime_error{dlerror()};
		return sym;
	}

	void* Dll::FindSymbol(const char* name) const noexcept
	{
		return dlsym(dll_.get(), name);
	}
	
	void CpuId(int cpu_info[4], int func_id) noexcept
	{
		auto* const info = reinterpret_cast<unsigned*>(cpu_info);
		const auto leaf = static_cast<unsigned>(func_id);
		__cpuid(leaf, info[0], info[1], info[2], info[3]);
	}
	
	void CpuIdEx(int cpu_info[4], int func_id, int sub_func_id) noexcept
	{
		auto* const info = reinterpret_cast<unsigned*>(cpu_info);
		const auto leaf = static_cast<unsigned>(func_id);
		const auto sub_leaf = static_cast<unsigned>(sub_func_id);
		__cpuid_count(leaf, sub_leaf, info[0], info[1], info[2], info[3]);
	}
}

#endif
