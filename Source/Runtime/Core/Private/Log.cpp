#include "Log.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Templates/Sync.hpp"
#include "Platform.hpp"

namespace oeng::log
{
#ifdef OE_LOG_THREADSAFE
	static constexpr bool kThreadSafe = true;
#else
	static constexpr bool kThreadSafe = false;
#endif

	static auto logger_initialized = false;
	static auto logger = spdlog::default_logger();
	
	void InitializeLogger()
	{
		assert(!logger_initialized);

		namespace fs = std::filesystem;
		const auto dir = plf::GetUserDataPath() / u8"Logs";
		create_directories(dir);

		using Daily = std::conditional_t<kThreadSafe,
			spdlog::sinks::daily_file_sink_mt,
			spdlog::sinks::daily_file_sink_st>;
		
		using Stdout = std::conditional_t<kThreadSafe,
			spdlog::sinks::stdout_color_sink_mt,
			spdlog::sinks::stdout_color_sink_st>;

		const auto filepath = dir / Format(u8"{}.log", GetGameName());
		auto daily_file = std::make_shared<Daily>(filepath.string(), 0, 0);
		auto stdout_color = std::make_shared<Stdout>();
		
		spdlog::sinks_init_list list{std::move(daily_file), std::move(stdout_color)};
		logger = std::make_shared<spdlog::logger>(std::string{}, list);
		logger->flush_on(level::critical);
#ifndef NDEBUG
		logger->set_level(level::debug);
#endif
		logger_initialized = true;
	}

	OEAPI spdlog::logger& GetLogger()
	{
		return *logger;
	}

	namespace detail
	{
		static auto& GetLogs()
		{
			using LogMap = std::unordered_map<unsigned, TimePoint>;
			static CondMonitor<LogMap, kThreadSafe> logs;
			return logs;
		}
		
		LogDelay::LogDelay() noexcept
		{
			static CondAtomic<unsigned, kThreadSafe> id = 0u;
			id_ = id++;
		}

		void LogDelay::operator()(Duration delay, level::level_enum level, std::u8string_view msg) const
		{
			{
				const auto logs = GetLogs().Lock();
				auto& next = (*logs)[id_];
				const auto now = Clock::now();
				if (next > now) return;
				next = now + delay;
			}
			Log(level, msg);
		}
	}
}
