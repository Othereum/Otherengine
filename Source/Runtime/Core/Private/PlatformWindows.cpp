#ifdef _WIN32

#define NOMINMAX

#include <intrin.h>
#include <ShlObj.h>
#include <Windows.h>
#include "EngineBase.hpp"
#include "Format.hpp"
#include "Platform.hpp"
#include "Templates/String.hpp"

namespace oeng::core
{
#ifdef NDEBUG
	fs::path GetUserDataPath()
	{
		assert(kEngineBase);

		wchar_t* wide;
		SHGetKnownFolderPath(FOLDERID_Documents, 0, nullptr, &wide);
		std::unique_ptr<wchar_t[], void(*)(void*)> wide_raii{wide, &CoTaskMemFree};
		
		return fs::path{wide} /= kEngineBase->GetGameName();
	}
#else
	bool detail::IsDebuggingImpl() noexcept
	{
		return IsDebuggerPresent();
	}
#endif
	
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
		
		return ToUtf8<std::allocator<char8_t>>({reinterpret_cast<char16_t*>(buffer), len});
	}

	static void FreeDll(void* dll) noexcept
	{
		FreeLibrary(static_cast<HMODULE>(dll));
	}
	
	Dll::Dll(std::u8string filepath)
	{
		auto* const dll = LoadLibraryW(LPCWSTR(ToUtf16(filepath).c_str()));
		if (!dll) Throw(u8"{}: cannot load module: {}"sv, filepath, GetLastErrStr());

		dll_.reset(dll, &FreeDll);
		filepath_ = std::move(filepath);
	}

	void* Dll::GetSymbol(std::u8string_view name) const
	{
		auto* const symbol = FindSymbol(name);
		if (!symbol) Throw(u8"{}: {}: {}"sv, filepath_, name, GetLastErrStr());
		return symbol;
	}

	void* Dll::FindSymbol(std::u8string_view name) const noexcept
	{
		return reinterpret_cast<void*>(GetProcAddress(HMODULE(dll_.get()), LPCSTR(name.data())));
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