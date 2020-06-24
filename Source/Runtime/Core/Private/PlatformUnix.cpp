#ifdef __unix__

#include "Platform.hpp"
#include <cpuid.h>
#include <dlfcn.h>

namespace oeng::plf
{
	Dll::Dll(const char* filepath) try
		:dll_{dlopen(filepath, RTLD_LAZY), &dlclose}
	{
	}
	catch (const std::invalid_argument&)
	{
		throw std::runtime_error{dlerror()};
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
