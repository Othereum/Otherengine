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
#include "Templates/Time.hpp"

namespace oeng::log
{
	namespace level = spdlog::level;
	
	OEAPI spdlog::logger& GetLogger();

	template <class... Args>
	void Log(level::level_enum level, std::string_view fmt, const Args&... args)
	{
		GetLogger().log(level, fmt, args...);
	}
	
	template <class... Args>
	void Trace(std::string_view fmt, const Args&... args)
	{
		GetLogger().trace(fmt, args...);
	}

	template <class... Args>
	void Debug(std::string_view fmt, const Args&... args)
	{
		GetLogger().debug(fmt, args...);
	}

	template <class... Args>
	void Info(std::string_view fmt, const Args&... args)
	{
		GetLogger().info(fmt, args...);
	}

	template <class... Args>
	void Warn(std::string_view fmt, const Args&... args)
	{
		GetLogger().warn(fmt, args...);
	}

	template <class... Args>
	void Error(std::string_view fmt, const Args&... args)
	{
		GetLogger().error(fmt, args...);
	}

	template <class... Args>
	void Critical(std::string_view fmt, const Args&... args)
	{
		GetLogger().critical(fmt, args...);
	}

	OEAPI void LogDelay(Duration delay, level::level_enum level, std::string_view msg);

	namespace detail
	{
		class OEAPI LogDelay
		{
		public:
			LogDelay() noexcept;
			
			void operator()(Duration delay, level::level_enum level, std::string_view msg) const;

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, level::level_enum level, std::string_view msg) const
			{
				operator()(time::duration_cast<Duration>(delay), level, msg);
			}

			template <class Rep, class Period, class... Args>
			void operator()(time::duration<Rep, Period> delay, level::level_enum level, std::string_view fmt, const Args&... args) const
			{
				operator()(time::duration_cast<Duration>(delay), level, fmt::format(fmt, args...));
			}

		private:
			unsigned id_;
		};
	}
}

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif

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

#ifdef __clang__
#pragma clang diagnostic pop
#endif
