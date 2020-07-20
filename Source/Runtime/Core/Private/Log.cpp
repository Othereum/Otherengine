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
#include "EngineBase.hpp"
#include "Platform.hpp"
#include "Templates/Sync.hpp"

namespace oeng::core::log
{
	static_assert(Level::kTrace == Level(spdlog::level::trace));
	static_assert(Level::kDebug == Level(spdlog::level::debug));
	static_assert(Level::kInfo == Level(spdlog::level::info));
	static_assert(Level::kWarn == Level(spdlog::level::warn));
	static_assert(Level::kErr == Level(spdlog::level::err));
	static_assert(Level::kCritical == Level(spdlog::level::critical));
	static_assert(Level::kOff == Level(spdlog::level::off));
	
	LogManager::LogManager()
	{
		assert(kEngineBase);
		
		using Daily = std::conditional_t<kLogThreadSafe,
			spdlog::sinks::daily_file_sink_mt,
			spdlog::sinks::daily_file_sink_st>;
		
		using Stdout = std::conditional_t<kLogThreadSafe,
			spdlog::sinks::stdout_color_sink_mt,
			spdlog::sinks::stdout_color_sink_st>;

		auto dir = GetUserDataPath() /= u8"Logs";
		create_directories(dir);

		dir /= Format(u8"{}.log"sv, kEngineBase->GetGameName());
		auto daily_file = std::make_shared<Daily>(dir.string(), 0, 0);
		auto stdout_color = std::make_shared<Stdout>();
		
		spdlog::sinks_init_list list{std::move(daily_file), std::move(stdout_color)};
		logger_ = std::make_shared<spdlog::logger>(std::string{}, list);
		logger_->flush_on(spdlog::level::critical);
		
#ifndef NDEBUG
		logger_->set_level(spdlog::level::debug);
#endif
	}

	void LogManager::Log(Level level, std::u8string_view message) const
	{
		logger_->log(spdlog::level::level_enum(level), AsString(message));
	}

	void LogManager::LogDelay(unsigned id, Duration delay, Level level, std::u8string_view msg)
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
		if (kEngineBase) LIKELY
		{
			kEngineBase->GetLogger().Log(level, message);
		}
		else
		{
			spdlog::log(spdlog::level::level_enum(level), AsString(message));
		}
	}

	namespace detail
	{
		LogDelay::LogDelay() noexcept
		{
			static CondAtomic<unsigned, kLogThreadSafe> id = 0u;
			id_ = id++;
		}

		void LogDelay::operator()(Duration delay, Level level, std::u8string_view msg) const
		{
			if (kEngineBase) LIKELY
			{
				kEngineBase->GetLogger().LogDelay(id_, delay, level, msg);
			}
			else
			{
				spdlog::log(spdlog::level::level_enum(level), AsString(msg));
			}
		}
	}
}
