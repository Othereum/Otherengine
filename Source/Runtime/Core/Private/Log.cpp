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

class Logger
{
public:
    [[nodiscard]] static Logger& Get()
    {
        static Logger logger;
        return logger;
    }

    void Init()
    {
        if (!file_.sinks().empty())
            return;

        auto dir = GetUserDataPath();
        dir /= u8"Logs"sv;

        create_directories(dir);
        dir /= fmt::format(u8"{}.log"sv, GetGameName());

        file_.sinks().push_back(std::make_shared<FileSink>(dir.string(), 0, 0));
        file_.set_level(spdlog::level::debug);
        file_.flush_on(spdlog::level::critical);
        console_.flush_on(spdlog::level::critical);
    }

    void Flush()
    {
        console_.flush();
        file_.flush();
    }

    void Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message)
    {
        const auto msg = fmt::format(u8"[{}] {}"sv, *category.name, message);
        file_.log(ToSpdLogLevel(level), AsString(msg));

#ifdef NDEBUG
	    if (level != LogLevel::kLog)
#endif
        {
            console_.log(ToSpdLogLevel(level), AsString(msg));
        }
    }

    void LogDelay(size_t id, Duration delay, const logcat::LogCategory& category, LogLevel level,
                  std::u8string_view message)
    {
        {
            const auto logs = delayed_.Lock();
            auto& next = (*logs)[id];

            const auto now = Clock::now();
            if (next > now)
                return;

            next = now + delay;
        }

        Log(category, level, message);
    }

private:
    using ConsoleSink = std::conditional_t<kLogThreadSafe, spdlog::sinks::stdout_color_sink_mt,
                                           spdlog::sinks::stdout_color_sink_st>;
    using FileSink = std::conditional_t<kLogThreadSafe, spdlog::sinks::daily_file_sink_mt,
                                        spdlog::sinks::daily_file_sink_st>;

    spdlog::logger console_{{}, std::make_shared<ConsoleSink>()};
    spdlog::logger file_{{}};
    CondMonitor<std::unordered_map<size_t, TimePoint>, kLogThreadSafe> delayed_;
};

// Called by SetGameName()
void InitLogger()
{
    Logger::Get().Init();
}

void FlushLog()
{
    Logger::Get().Flush();
}

void Log(const logcat::LogCategory& category, LogLevel level, std::u8string_view message)
{
    Logger::Get().Log(category, level, message);
}

DelayedLog::DelayedLog()
{
    static CondAtomic<size_t, kLogThreadSafe> id = 0u;
    id_ = ++id;
}

void DelayedLog::operator()(Duration delay, const logcat::LogCategory& category, LogLevel level,
                            std::u8string_view message) const
{
    Logger::Get().LogDelay(id_, delay, category, level, message);
}
}
}
