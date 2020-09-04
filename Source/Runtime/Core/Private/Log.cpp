#include "Log.hpp"
#include <filesystem>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace oeng
{
inline namespace core
{
#ifdef OE_LOG_THREADSAFE
static constexpr auto kLogThreadSafe = true;
#else
static constexpr auto kLogThreadSafe = false;
#endif

using ConsoleSink = std::conditional_t<kLogThreadSafe, spdlog::sinks::stdout_color_sink_mt,
                                       spdlog::sinks::stdout_color_sink_st>;

using FileSink = std::conditional_t<kLogThreadSafe, spdlog::sinks::daily_file_sink_mt,
                                    spdlog::sinks::daily_file_sink_st>;

static spdlog::logger console{{}, std::make_shared<ConsoleSink>()};
static spdlog::logger file{{}};
CondMonitor<std::unordered_map<size_t, TimePoint>, kLogThreadSafe> delayed;

void InitLogger()
{
    if (!file.sinks().empty())
        return;

    auto dir = GetUserDataPath();
    dir /= u8"Logs"sv;

    create_directories(dir);
    dir /= fmt::format(u8"{}.log"sv, GetGameName());

    file.sinks().push_back(std::make_shared<FileSink>(dir.string(), 0, 0));
    file.set_level(spdlog::level::debug);
    file.flush_on(spdlog::level::critical);
    console.flush_on(spdlog::level::critical);
}

void FlushLog()
{
    console.flush();
    file.flush();
}

[[nodiscard]] static constexpr spdlog::level::level_enum ToSpdLogLevel(LogLevel level)
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

void Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message)
{
    const auto msg = fmt::format(u8"[{}] {}"sv, category.name, message);
    file.log(ToSpdLogLevel(level), AsString(msg));

#ifdef NDEBUG
	if (level != LogLevel::kLog)
#endif
    {
        console.log(ToSpdLogLevel(level), AsString(msg));
    }
}

DelayedLog::DelayedLog()
{
    static CondAtomic<size_t, kLogThreadSafe> id = 0u;
    id_ = id++;
}

void DelayedLog::operator()(Duration delay, const logcat::LogCategory& category, LogLevel level,
                            std::u8string_view msg) const
{
    {
        const auto logs = delayed.Lock();
        auto& next = (*logs)[id_];

        const auto now = Clock::now();
        if (next > now)
            return;

        next = now + delay;
    }

    Log(category, level, msg);
}
}
}
