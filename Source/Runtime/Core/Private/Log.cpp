#include "Log.hpp"
#include <filesystem>
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4819)
#endif
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include "Platform.hpp"
#include "Templates/Sync.hpp"

namespace oeng::core::log
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

		const auto dir = GetUserDataPath() / u8"Logs";
		create_directories(dir);

		using Daily = std::conditional_t<kThreadSafe,
			spdlog::sinks::daily_file_sink_mt,
			spdlog::sinks::daily_file_sink_st>;
		
		using Stdout = std::conditional_t<kThreadSafe,
			spdlog::sinks::stdout_color_sink_mt,
			spdlog::sinks::stdout_color_sink_st>;

		const auto filepath = dir / Format(u8"{}.log"sv, GetGameName());
		auto daily_file = std::make_shared<Daily>(filepath.string(), 0, 0);
		auto stdout_color = std::make_shared<Stdout>();
		
		spdlog::sinks_init_list list{std::move(daily_file), std::move(stdout_color)};
		logger = std::make_shared<spdlog::logger>(std::string{}, list);
		logger->flush_on(spdlog::level::critical);
#ifndef NDEBUG
		logger->set_level(spdlog::level::debug);
#endif
		logger_initialized = true;
	}

	void Log(Level level, std::u8string_view message)
	{
		static_assert(Level::kTrace == Level(spdlog::level::trace));
		static_assert(Level::kDebug == Level(spdlog::level::debug));
		static_assert(Level::kInfo == Level(spdlog::level::info));
		static_assert(Level::kWarn == Level(spdlog::level::warn));
		static_assert(Level::kErr == Level(spdlog::level::err));
		static_assert(Level::kCritical == Level(spdlog::level::critical));
		static_assert(Level::kOff == Level(spdlog::level::off));
		logger->log(spdlog::level::level_enum(level), AsString(message));
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

		void LogDelay::operator()(Duration delay, Level level, std::u8string_view msg) const
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
