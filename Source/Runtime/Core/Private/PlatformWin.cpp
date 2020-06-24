#ifdef _WIN32

#include "Platform.hpp"
#include <Windows.h>
#include <intrin.h>

#undef min
#undef CpuIdEx

namespace oeng::plf
{
	[[noreturn]] static void ThrowLastError()
	{
		constexpr auto buffer_size = 1024;
		char buffer[buffer_size];
		FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr,
			GetLastError(), 0, buffer, buffer_size, nullptr);
		throw std::runtime_error{buffer};
	}

	static void FreeDll(void* dll) noexcept
	{
		FreeLibrary(static_cast<HMODULE>(dll));
	}
	
	Dll::Dll(const char* filepath)
		:dll_{LoadLibraryA(filepath), &FreeDll}
	{
		if (!dll_) ThrowLastError();
	}

	void* Dll::GetSymbol(const char* name) const
	{
		auto* const symbol = FindSymbol(name);
		if (!symbol) ThrowLastError();
		return symbol;
	}

	void* Dll::FindSymbol(const char* name) const noexcept
	{
		return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(dll_.get()), name));
	}

	void CpuId(int cpu_info[4], int func_id) noexcept
	{
		return __cpuid(cpu_info, func_id);
	}

	void CpuIdEx(int cpu_info[4], int func_id, int sub_func_id) noexcept
	{
		return __cpuidex(cpu_info, func_id, sub_func_id);
	}
}

#endif
