#pragma once
#include "Core.hpp"
#include "Format.hpp"
#include "Templates/Time.hpp"

namespace oeng::log
{
	enum class Level
	{
	    kTrace, kDebug, kInfo, kWarn, kErr, kCritical, kOff,
	};
	
	OEAPI void Log(Level level, std::u8string_view message);

	template <class... Args>
	void Log(Level level, std::u8string_view fmt, const Args&... args)
	{
		Log(level, Format(fmt, args...));
	}
	
	template <class... Args>
	void Trace(std::u8string_view fmt, const Args&... args)
	{
		Log(Level::kTrace, fmt, args...);
	}

	template <class... Args>
	void Debug(std::u8string_view fmt, const Args&... args)
	{
		Log(Level::kDebug, fmt, args...);
	}

	template <class... Args>
	void Info(std::u8string_view fmt, const Args&... args)
	{
		Log(Level::kInfo, fmt, args...);
	}

	template <class... Args>
	void Warn(std::u8string_view fmt, const Args&... args)
	{
		Log(Level::kWarn, fmt, args...);
	}

	template <class... Args>
	void Error(std::u8string_view fmt, const Args&... args)
	{
		Log(Level::kErr, fmt, args...);
	}

	template <class... Args>
	void Critical(std::u8string_view fmt, const Args&... args)
	{
		Log(Level::kCritical, fmt, args...);
	}

	OEAPI void LogDelay(Duration delay, Level level, std::u8string_view msg);

	namespace detail
	{
		class OEAPI LogDelay
		{
		public:
			LogDelay() noexcept;
			
			void operator()(Duration delay, Level level, std::u8string_view msg) const;

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, Level level, std::u8string_view msg) const
			{
				operator()(time::duration_cast<Duration>(delay), level, msg);
			}

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, Level level, std::u8string_view fmt, const Args&... args) const
			{
				operator()(time::duration_cast<Duration>(delay), level, Format(fmt, args...));
			}

		private:
			unsigned id_;
		};
	}
}

/**
 * \brief Log with delay (no spam)
 * \param delay time::duration
 * \param level log::level
 * \param format message format
 */
#define OE_DLOG(delay, level, format, ...) [&] \
{ \
	static const ::oeng::log::detail::LogDelay log_delay; \
	log_delay(delay, level, format, ##__VA_ARGS__); \
}()
