#ifdef _WIN32

#include <intrin.h>
#include <stdexcept>
#include <Windows.h>
#undef min
#undef CpuIdEx
#include "Format.hpp"
#include "Platform.hpp"

namespace oeng::plf
{
	bool IsDebugging() noexcept
	{
		return IsDebuggerPresent();
	}
	
	static std::string_view GetLastErrStr()
	{
		constexpr auto size = 1024;
		static char buffer[size];
		
		FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			GetLastError(),
			0,
			buffer,
			size,
			nullptr
		);
		
		return buffer;
	}

	static void FreeDll(void* dll) noexcept
	{
		FreeLibrary(static_cast<HMODULE>(dll));
	}
	
	Dll::Dll(const char* filepath)
	{
		auto* const dll = LoadLibraryA(filepath);
		
		if (!dll) throw std::runtime_error{format(
			"{}: cannot load module: {}", filepath, GetLastErrStr())};

		dll_.reset(dll, &FreeDll);
		filepath_ = filepath;
	}

	void* Dll::GetSymbol(const char* name) const
	{
		auto* const symbol = FindSymbol(name);
		
		if (!symbol) throw std::runtime_error{format(
			"{}: {}: {}", filepath_, name, GetLastErrStr())};
		
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