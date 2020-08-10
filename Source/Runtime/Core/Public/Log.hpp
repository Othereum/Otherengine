#pragma once
#include "Templates/Time.hpp"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace spdlog
{
	class logger;
}
#endif

namespace oeng::core
{
#ifdef OE_LOG_THREADSAFE
	constexpr auto kLogThreadSafe = true;
#else
	constexpr auto kLogThreadSafe = false;
#endif

	enum class LogLevel
	{
		/**
		 * Prints a debug message to log file if NDEBUG is not defined.
		 */
		kDebug,

		/**
		 * Prints a message to log file (and console if NDEBUG is not defined).
		 */
		kLog,

		/**
		 * Prints a message to console and log file.
		 */
		kDisplay,

		/**
		 * Prints a warning to console and log file.
		 */
		kWarn,
		
		/**
		 * Prints an error to console and log file.
		 */
		kErr,

		/**
		 * Prints a critical error to console and log file.
		 */
		kCritical
	};

	struct LogCategory
	{
		std::u8string_view name;
		LogLevel default_level = LogLevel::kLog;
		LogLevel min_level = LogLevel::kDebug;
	};

	class CORE_API Logger
	{
	public:
		void Log(const LogCategory& category, LogLevel level, std::u8string_view message) const;
		void LogDelay(unsigned id, Duration delay, const LogCategory& category, LogLevel level, std::u8string_view msg);
		
	private:
		friend class CoreSystem;
		Logger();
		
		std::shared_ptr<spdlog::logger> console_;
		std::shared_ptr<spdlog::logger> file_;

		// Should NOT use memory pool
		CondMonitor<std::unordered_map<unsigned, TimePoint>, kLogThreadSafe> delayed_;
	};
	
	CORE_API void Log(const LogCategory& category, LogLevel level, std::u8string_view message);

	template <class... Args>
	void Log(const LogCategory& category, LogLevel level, std::u8string_view fmt, const Args&... args)
	{
		Log(category, level, Format(fmt, args...));
	}
	
	template <class... Args>
	void Log(const LogCategory& category, std::u8string_view fmt, const Args&... args)
	{
		Log(category, category.default_level, fmt, args...);
	}
	
	namespace detail
	{
		class CORE_API LogDelay
		{
		public:
			LogDelay();
			
			void operator()(Duration delay, const LogCategory& category, LogLevel level, std::u8string_view msg) const;

			template <class Rep, class Period>
			void operator()(time::duration<Rep, Period> delay, const LogCategory& category, LogLevel level, std::u8string_view msg) const
			{
				operator()(time::duration_cast<Duration>(delay), category, level, msg);
			}

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, const LogCategory& category, LogLevel level, std::u8string_view fmt, const Args&... args) const
			{
				operator()(time::duration_cast<Duration>(delay), category, level, Format(fmt, args...));
			}

		private:
			unsigned id_;
		};
	}
}

/**
 * Log with delay (anti-spam)
 * @param delay Minimum interval
 * @param category Log category
 * @param level Log level
 * @param format Message format
 */
#define OE_DLOG(delay, category, level, format, ...) [&] \
{ \
	static const ::oeng::log::detail::LogDelay log_delay; \
	log_delay(delay, category, level, format, ##__VA_ARGS__); \
}()
