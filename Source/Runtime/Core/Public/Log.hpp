#pragma once
#include "Core.hpp"
#include "Format.hpp"
#include "Templates/HashMap.hpp"
#include "Templates/Sync.hpp"
#include "Templates/Time.hpp"

/**
 * Try to execute the expression; if an exception thrown, swallow the exception if NDEBUG is defined; otherwise, abort the app.
 */
#define ASSERT_TRY(expr) do { try { expr; } catch (...) { assert(!"Critical exception"); } } while (false)

#ifndef DOXYGEN_SHOULD_SKIP_THIS
namespace spdlog
{
	class logger;
}
#endif

namespace oeng::core
{
	class CoreSystem;
}

namespace oeng::core::log
{
#ifdef OE_LOG_THREADSAFE
	constexpr auto kLogThreadSafe = true;
#else
	constexpr auto kLogThreadSafe = false;
#endif

	enum class Level
	{
	    kTrace, kDebug, kInfo, kWarn, kErr, kCritical, kOff,
	};

	class CORE_API Logger
	{
	public:
		void Log(Level level, std::u8string_view message) const noexcept;
		void LogDelay(unsigned id, Duration delay, Level level, std::u8string_view msg) noexcept;
		
	private:
		friend CoreSystem;
		Logger();
		
		std::shared_ptr<spdlog::logger> logger_;

		// Should not use memory pool
		CondMonitor<std::unordered_map<unsigned, TimePoint>, kLogThreadSafe> delayed_;
	};
	
	CORE_API void Log(Level level, std::u8string_view message) noexcept;

	template <class... Args>
	void Log(Level level, std::u8string_view fmt, const Args&... args) noexcept
	{
		ASSERT_TRY(Log(level, Format(fmt, args...)));
	}
	
	template <class... Args>
	void Trace(std::u8string_view fmt, const Args&... args) noexcept
	{
		Log(Level::kTrace, fmt, args...);
	}

	template <class... Args>
	void Debug(std::u8string_view fmt, const Args&... args) noexcept
	{
		Log(Level::kDebug, fmt, args...);
	}

	template <class... Args>
	void Info(std::u8string_view fmt, const Args&... args) noexcept
	{
		Log(Level::kInfo, fmt, args...);
	}

	template <class... Args>
	void Warn(std::u8string_view fmt, const Args&... args) noexcept
	{
		Log(Level::kWarn, fmt, args...);
	}

	template <class... Args>
	void Error(std::u8string_view fmt, const Args&... args) noexcept
	{
		Log(Level::kErr, fmt, args...);
	}

	template <class... Args>
	void Critical(std::u8string_view fmt, const Args&... args) noexcept
	{
		Log(Level::kCritical, fmt, args...);
	}

	namespace detail
	{
		class CORE_API LogDelay
		{
		public:
			LogDelay() noexcept;
			
			void operator()(Duration delay, Level level, std::u8string_view msg) const noexcept;

			template <class Rep, class Period>
			void operator()(time::duration<Rep, Period> delay, Level level, std::u8string_view msg) const noexcept
			{
				operator()(time::duration_cast<Duration>(delay), level, msg);
			}

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, Level level, std::u8string_view fmt, const Args&... args) const noexcept
			{
				operator()(time::duration_cast<Duration>(delay), level, Format(fmt, args...));
			}

		private:
			unsigned id_;
		};
	}
}

/**
 * Log with delay (anti-spam)
 * @param delay time::duration
 * @param level log::level
 * @param format message format
 */
#define OE_DLOG(delay, level, format, ...) [&] \
{ \
	static const ::oeng::log::detail::LogDelay log_delay; \
	log_delay(delay, level, format, ##__VA_ARGS__); \
}()
