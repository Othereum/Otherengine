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

namespace oeng::core::log
{
	[[nodiscard]] static spdlog::level::level_enum ToSpdLogLevel(Level level)
	{
		using namespace spdlog::level;
		
		switch (level)
		{
		case Level::kDebug:
			return debug;
			
		case Level::kLog:
		case Level::kDisplay:
			return info;

		case Level::kWarn:
			return warn;
			
		case Level::kErr:
			return err;
			
		case Level::kCritical:
			return critical;
			
		default:
			throw std::invalid_argument{"Invalid log level"};
		}
	}

	[[nodiscard]] static constexpr bool ShouldLogFile(Level level) noexcept
	{
#ifdef NDEBUG
		switch (level)
		{
		case Level::kDebug:
			return false;

		default: ;
		}
#endif
		
		return true;
	}
	
	[[nodiscard]] static constexpr bool ShouldLogConsole(Level level) noexcept
	{
#ifdef NDEBUG
		switch (level)
		{
		case Level::kDebug:
		case Level::kLog:
			return false;

		default: ;
		}
#endif

		return true;
	}
	
	Logger::Logger()
	{
		auto dir = GetUserDataPath() /= u8"Logs"sv;
		create_directories(dir);

		// Should NOT use memory pool
		dir /= fmt::format(u8"{}.log"sv, EngineBase::Get().GetGameName());

		if constexpr (kLogThreadSafe)
		{
			console_ = spdlog::stdout_color_mt({});
			file_ = spdlog::daily_logger_mt({}, dir.string());
		}
		else
		{
			console_ = spdlog::stdout_color_st({});
			file_ = spdlog::daily_logger_st({}, dir.string());
		}

		console_->set_level(spdlog::level::debug);
		file_->set_level(spdlog::level::debug);
	}

	void Logger::Log(Level level, std::u8string_view message) const
	{
		if (ShouldLogFile(level))
			file_->log(ToSpdLogLevel(level), AsString(message));

		if (ShouldLogConsole(level))
			console_->log(ToSpdLogLevel(level), AsString(message));
	}

	void Logger::LogDelay(unsigned id, Duration delay, Level level, std::u8string_view msg)
	{
		{
			const auto logs = delayed_.Lock();
			auto& next = (*logs)[id];
			
			const auto now = Clock::now();
			if (next > now) return;
			
			next = now + delay;
		}
		
		Log(level, msg);
	}

	void Log(Level level, std::u8string_view message)
	{
		if (EngineBase::Exists())
		{
			EngineBase::Get().GetLogger().Log(level, message);
		}
		else
		{
			spdlog::log(ToSpdLogLevel(level), AsString(message));
		}
	}

	namespace detail
	{
		LogDelay::LogDelay()
		{
			static CondAtomic<unsigned, kLogThreadSafe> id = 0u;
			id_ = id++;
		}

		void LogDelay::operator()(Duration delay, Level level, std::u8string_view msg) const
		{
			if (EngineBase::Exists())
			{
				EngineBase::Get().GetLogger().LogDelay(id_, delay, level, msg);
			}
			else
			{
				spdlog::log(ToSpdLogLevel(level), AsString(msg));
			}
		}
	}
}
