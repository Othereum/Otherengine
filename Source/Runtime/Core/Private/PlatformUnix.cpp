#ifdef __unix__

#include <cpuid.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdexcept>
#include <unistd.h>
#include "EngineBase.hpp"
#include "Platform.hpp"
#include "Templates/String.hpp"

namespace oeng::core
{
#ifdef NDEBUG
	fs::path GetUserDataPath()
	{
		fs::path p = std::getenv("HOME");
		p /= u8".";
		p += EngineBase::Get().GetGameName();
		return p;
	}
#else
	bool detail::IsDebuggingImpl() noexcept
	{
		const auto status_file = open("/proc/self/status", O_RDONLY);
		if (status_file == -1) return false;

		char buffer[256];
		const auto len = read(status_file, buffer, sizeof buffer);

		auto is_debugging = false;
		constexpr auto& tracer_str = "TracerPid:\t";
		constexpr ssize_t len_tracer_str = sizeof tracer_str - 1;
		
		for (auto i = 0; len - i > len_tracer_str; ++i)
		{
			if (std::strncmp(buffer + i, tracer_str, len_tracer_str) == 0)
			{
				is_debugging = buffer[i + len_tracer_str] != '0';
				break;
			}
		}

		close(status_file);
		return is_debugging;
	}
#endif
	
	Dll::Dll(std::u8string filepath)
	{
		const auto dll = dlopen(AsString(filepath).c_str(), RTLD_LAZY);
		if (!dll) throw std::runtime_error{dlerror()};

		dll_.reset(dll, &dlclose);
		filepath_ = std::move(filepath);
	}

	void* Dll::GetSymbol(std::u8string_view name) const
	{
		auto* const sym = FindSymbol(name);
		if (!sym) throw std::runtime_error{dlerror()};
		return sym;
	}

	void* Dll::FindSymbol(std::u8string_view name) const noexcept
	{
		return dlsym(dll_.get(), AsString(name).data());
	}
	
	void CpuId(int cpu_info[4], int func_id) noexcept
	{
		auto* const info = reinterpret_cast<unsigned*>(cpu_info);
		const auto leaf = static_cast<unsigned>(func_id);
		__cpuid(leaf, info[0], info[1], info[2], info[3]);
	}
	
	void CpuIdCnt(int cpu_info[4], int func_id, int sub_func_id) noexcept
	{
		auto* const info = reinterpret_cast<unsigned*>(cpu_info);
		const auto leaf = static_cast<unsigned>(func_id);
		const auto sub_leaf = static_cast<unsigned>(sub_func_id);
		__cpuid_count(leaf, sub_leaf, info[0], info[1], info[2], info[3]);
	}
}

#endif
