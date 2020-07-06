#ifdef _WIN32

#define NOMINMAX

#include <intrin.h>
#include <stdexcept>
#include <Windows.h>
#include "Format.hpp"
#include "Platform.hpp"
#include "Templates/String.hpp"

namespace oeng::plf
{
	bool IsDebugging() noexcept
	{
		return IsDebuggerPresent();
	}
	
	static std::u8string GetLastErrStr()
	{
		constexpr auto size = 512;
		static wchar_t buffer[size];

		const auto len = FormatMessageW(
			FORMAT_MESSAGE_FROM_SYSTEM,
			nullptr,
			GetLastError(),
			0,
			buffer,
			size,
			nullptr
		);
		
		return ToUtf8({reinterpret_cast<char16_t*>(buffer), len});
	}

	static void FreeDll(void* dll) noexcept
	{
		FreeLibrary(static_cast<HMODULE>(dll));
	}
	
	Dll::Dll(const char8_t* filepath)
	{
		auto* const dll = LoadLibraryW(LPCWSTR(ToUtf16(filepath).data()));
		
		if (!dll) Throw(u8"{}: cannot load module: {}", filepath, GetLastErrStr());

		dll_.reset(dll, &FreeDll);
		filepath_ = filepath;
	}

	void* Dll::GetSymbol(const char8_t* name) const
	{
		auto* const symbol = FindSymbol(name);
		if (!symbol) Throw(u8"{}: {}: {}", filepath_, name, GetLastErrStr());
		return symbol;
	}

	void* Dll::FindSymbol(const char8_t* name) const noexcept
	{
		return reinterpret_cast<void*>(GetProcAddress(HMODULE(dll_.get()), LPCSTR(name)));
	}

	void CpuId(int cpu_info[4], int func_id) noexcept
	{
		return __cpuid(cpu_info, func_id);
	}

	void CpuIdCnt(int cpu_info[4], int func_id, int sub_func_id) noexcept
	{
		return __cpuidex(cpu_info, func_id, sub_func_id);
	}
}

#endif