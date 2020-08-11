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

namespace oeng::core
{
	[[nodiscard]] static spdlog::level::level_enum ToSpdLogLevel(LogLevel level)
	{
		switch (level)
		{
		case LogLevel::kDebug:
			return spdlog::level::debug;
			
		case LogLevel::kLog:
		case LogLevel::kDisplay:
			return spdlog::level::info;

		case LogLevel::kWarn:
			return spdlog::level::warn;
			
		case LogLevel::kErr:
			return spdlog::level::err;
			
		case LogLevel::kCritical:
			return spdlog::level::critical;
			
		default:
			throw std::invalid_argument{"Invalid log level"};
		}
	}

	Logger::Logger()
	{
		auto dir = GetUserDataPath();
		dir /= u8"Logs"sv;
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

#ifndef NDEBUG
		file_->set_level(spdlog::level::debug);
#endif
	}

	void Logger::Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message) const
	{
		const auto msg = Format(u8"[{}] {}"sv, category.name, message);
		file_->log(ToSpdLogLevel(level), AsString(msg));

#ifdef NDEBUG
		if (level != LogLevel::kLog)
#endif
		{
			console_->log(ToSpdLogLevel(level), AsString(msg));
		}
	}

	void Logger::LogDelay(unsigned id, Duration delay, const logcat::LogCategory& category, LogLevel level, std::u8string_view msg)
	{
		{
			const auto logs = delayed_.Lock();
			auto& next = (*logs)[id];
			
			const auto now = Clock::now();
			if (next > now) return;
			
			next = now + delay;
		}
		
		Log(category, level, msg);
	}

	void Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message)
	{
		if (EngineBase::Exists())
		{
			EngineBase::Get().GetLogger().Log(category, level, message);
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

		void LogDelay::operator()(Duration delay, const logcat::LogCategory& category, LogLevel level, std::u8string_view msg) const
		{
			if (EngineBase::Exists())
			{
				EngineBase::Get().GetLogger().LogDelay(id_, delay, category, level, msg);
			}
			else
			{
				spdlog::log(ToSpdLogLevel(level), AsString(msg));
			}
		}
	}
}
