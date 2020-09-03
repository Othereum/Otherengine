#include "Log.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace oeng
{
inline namespace core
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
    using ConsoleSink = std::conditional_t<kLogThreadSafe, spdlog::sinks::stdout_color_sink_mt,
                                           spdlog::sinks::stdout_color_sink_st>;
    using FileSink = std::conditional_t<kLogThreadSafe, spdlog::sinks::daily_file_sink_mt,
                                        spdlog::sinks::daily_file_sink_st>;

    auto dir = GetUserDataPath();
    dir /= u8"Logs"sv;
    create_directories(dir);
    dir /= fmt::format(u8"{}.log"sv, EngineBase::Get().GetGameName());

    console_ = std::make_shared<spdlog::logger>(std::string{}, std::make_shared<ConsoleSink>());
    file_ = std::make_shared<spdlog::logger>(std::string{}, std::make_shared<FileSink>(dir.string(), 0, 0));

#ifndef NDEBUG
    file_->set_level(spdlog::level::debug);
#endif
}

void Logger::Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message) const
{
    const auto msg = fmt::format(u8"[{}] {}"sv, category.name, message);
    file_->log(ToSpdLogLevel(level), AsString(msg));

#ifdef NDEBUG
		if (level != LogLevel::kLog)
#endif
    {
        console_->log(ToSpdLogLevel(level), AsString(msg));
    }
}

void Logger::LogDelay(unsigned id, Duration delay, const logcat::LogCategory& category, LogLevel level,
                      std::u8string_view msg)
{
    {
        const auto logs = delayed_.Lock();
        auto& next = (*logs)[id];

        const auto now = Clock::now();
        if (next > now)
            return;

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

DelayedLog::DelayedLog()
{
    static CondAtomic<unsigned, kLogThreadSafe> id = 0u;
    id_ = id++;
}

void DelayedLog::operator()(Duration delay, const logcat::LogCategory& category, LogLevel level,
                            std::u8string_view msg) const
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
