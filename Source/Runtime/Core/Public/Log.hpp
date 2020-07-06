#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4566 4819)
#endif
#include <spdlog/logger.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include "Core.hpp"
#include "Format.hpp"
#include "Templates/Time.hpp"

namespace oeng::log
{
	namespace level = spdlog::level;
	
	OEAPI spdlog::logger& GetLogger();

	template <FmtStr S, class... Args>
	void Log(level::level_enum level, const S& fmt, Args&&... args)
	{
		auto str = Format(fmt, std::forward<Args>(args)...);
		GetLogger().log(level, reinterpret_cast<std::string&&>(str));
	}
	
	template <FmtStr S, class... Args>
	void Trace(const S& fmt, const Args&... args)
	{
		Log(level::trace, fmt, args...);
	}

	template <FmtStr S, class... Args>
	void Debug(const S& fmt, const Args&... args)
	{
		Log(level::debug, fmt, args...);
	}

	template <FmtStr S, class... Args>
	void Info(const S& fmt, const Args&... args)
	{
		Log(level::info, fmt, args...);
	}

	template <FmtStr S, class... Args>
	void Warn(const S& fmt, const Args&... args)
	{
		Log(level::warn, fmt, args...);
	}

	template <FmtStr S, class... Args>
	void Error(const S& fmt, const Args&... args)
	{
		Log(level::err, fmt, args...);
	}

	template <FmtStr S, class... Args>
	void Critical(const S& fmt, const Args&... args)
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
			void operator()(time::duration<Rep, Period> delay, level::level_enum level, std::u8string_view fmt, Args&&... args) const
			{
				operator()(time::duration_cast<Duration>(delay), level, Format(fmt, std::forward<Args>(args)...));
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
