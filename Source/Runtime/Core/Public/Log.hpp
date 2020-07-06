#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4566 4819)
#endif
#include <spdlog/logger.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include "Templates/String.hpp"
#include "Templates/Time.hpp"

namespace oeng::log
{
	namespace level = spdlog::level;
	
	OEAPI spdlog::logger& GetLogger();

	template <class... Args>
	void Log(level::level_enum level, std::u8string_view fmt, const Args&... args)
	{
		GetLogger().log(level, reinterpret_cast<std::string_view&&>(fmt), args...);
	}
	
	template <class... Args>
	void Trace(std::u8string_view fmt, const Args&... args)
	{
		Log(level::trace, fmt, args...);
	}

	template <class... Args>
	void Debug(std::u8string_view fmt, const Args&... args)
	{
		Log(level::debug, fmt, args...);
	}

	template <class... Args>
	void Info(std::u8string_view fmt, const Args&... args)
	{
		Log(level::info, fmt, args...);
	}

	template <class... Args>
	void Warn(std::u8string_view fmt, const Args&... args)
	{
		Log(level::warn, fmt, args...);
	}

	template <class... Args>
	void Error(std::u8string_view fmt, const Args&... args)
	{
		Log(level::err, fmt, args...);
	}

	template <class... Args>
	void Critical(std::u8string_view fmt, const Args&... args)
	{
		Log(level::critical, fmt, args...);
	}

	OEAPI void LogDelay(Duration delay, level::level_enum level, std::u8string_view msg);

	namespace detail
	{
		class OEAPI LogDelay
		{
		public:
			LogDelay() noexcept;
			
			void operator()(Duration delay, level::level_enum level, std::u8string_view msg) const;

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, level::level_enum level, std::u8string_view msg) const
			{
				operator()(time::duration_cast<Duration>(delay), level, msg);
			}

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, level::level_enum level, std::u8string_view fmt, const Args&... args) const
			{
				operator()(time::duration_cast<Duration>(delay), level, fmt::format(fmt, args...));
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
